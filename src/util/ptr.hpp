#pragma once

#include <memory>
#include <exception>

namespace ve
{
	//! This is a function that can be passed to OwnPtr to delete arrays properly.
	template <typename T> void deleteArray(T * p);

	//! This is the standard delete function for OwnPtr. It just calls the delete operator.
	template <typename T> void deleteObject(T * p);

	//! An exception for accessing objects that don't exist (null pointers, bad pointers, etc).
	class bad_dereference_exception;

	// Forward declaration for internal counting class.
	class _PtrCounter;

	// A base for the OwnPtr and Ptr classes for shared function. This class cannot be instantiated on its own.
	template <typename T>
	class PtrBase
	{
	public:
		//! Returns true if this points to something non-null.
		bool isValid() const;

		//! Provides access to the pointee's members.
		T * operator -> () const;

		//! Provides access to the element located at index. Warning: this provides no index out-of-bounds checking for efficiency.
		T & operator [] (int index) const;

		//! Provides reference access to the pointee's members.
		T & operator * () const;

		//! Only to be used for functions that require a raw pointer to the pointee. Be careful how you use this.
		T * raw() const;

		//! Get the unsigned integer value of the address of the pointee.
		operator intptr_t() const;

		//! Returns true if the address of the pointee is less than the address of ptr's pointee.
		template <typename Y> bool operator < (PtrBase<Y> const & ptr) const;

		//! Returns true if the address of the pointee is equal to the address of ptr's pointee.
		template <typename Y> bool operator == (PtrBase<Y> const & ptr) const;

	protected:
		PtrBase();

		template <typename Y> PtrBase(Y * p, _PtrCounter * c);

		~PtrBase() {}

		T * p = nullptr;
		_PtrCounter * c = nullptr;

		template<typename Y> friend class PtrBase;
	};

	//! A unique smart pointer that can pass out Ptrs for use by others. It is not bound by the other Ptrs. See rationale at the end of the file.
	template <typename T>
	class OwnPtr final : public PtrBase<T>
	{
	public:
		//! Default constructor. Initializes this to null.
		OwnPtr();

		//! Move constructor. Moves ownership of ownPtr's pointee to this.
		OwnPtr(OwnPtr<T> && ownPtr);

		//! Templated move constructor. Moves ownership of ownPtr's pointee to this. Y must be a subclass of T.
		template <typename Y> OwnPtr(OwnPtr<Y> && ownPtr);

		//! Move assignment operator. Moves ownership of ownPtr's pointee to this and returns this.
		OwnPtr<T> & operator = (OwnPtr<T> && ownPtr);

		//! Templated move assignment operator. Moves ownership of ownPtr's pointee to this and returns this. Y must be a subclass of T.
		template <typename Y> OwnPtr<T> & operator = (OwnPtr<Y> && ownPtr);

		//! Destructor.
		~OwnPtr();

		//! Returns the number of pointers that refer to the pointee.
		int numPtrs() const;

		//! Resets this to point to null.
		void setNull();

		//! Point the pointer to newP, which can have a type that is subclass of T. Only pass in something that looks like 'new T()' to ensure that the raw pointer isn't used elsewhere.
		template <typename Y> void setRaw(Y * newP, void(*deleteFunction) (Y *) = deleteObject);

		//! Change the object to a new pointer to an object of type T with arguments. Uses the new operator for allocation. For special allocation use the function setRaw().
		template <typename ... Args> void setNew(Args && ... args);

		//! Change the object to a new pointer to an object of type Y with arguments. Uses the new operator for allocation. For special allocation the function setRaw().
		template <typename Y, typename ... Args> void setNew(Args && ... args);

		//! Returns a newly created OwnPtr using setNew above.
		template <typename ... Args> static OwnPtr<T> returnNew(Args && ... args);

		template<typename Y> friend class OwnPtr;
		template<typename Y> friend class Ptr;
	};

	// A copyable referencing smart pointer acquired from an OwnPtr. See rationale at the end of the file.
	template <typename T>
	class Ptr final : public PtrBase<T>
	{
	public:
		//! Default constructor. Initializes this to null.
		Ptr();

		//! Copy constructor.
		Ptr(Ptr<T> const & ptr);

		//! Copy constructor from OwnPtrs.
		Ptr(OwnPtr<T> const & ownPtr);

		//! Templated copy constructor. Y must be a subclass of T.
		template <typename Y> Ptr(Ptr<Y> const & ptr);

		//! Templated copy constructor from OwnPtrs. Y must be a subclass of T.
		template <typename Y> Ptr(OwnPtr<Y> const & ownPtr);

		//! Copy assignment operator.
		Ptr<T> & operator = (Ptr<T> const & ptr);

		//! Copy assignment operator from OwnPtrs.
		Ptr<T> & operator = (OwnPtr<T> const & ownPtr);

		//! Templated copy assignment operator. Y must be a subclass of T.
		template <typename Y> Ptr<Y> & operator = (Ptr<Y> const & ptr);

		//! Templated copy assignment operator from OwnPtrs. Y must be a subclass of T.
		template <typename Y> Ptr<Y> & operator = (OwnPtr<Y> const & ownPtr);

		//! Destructor.
		~Ptr();

		//! Resets this to point to null.
		void setNull();

		//! Returns a Ptr dynamically casted to Y. Y must be a superclass of T.
		template <typename Y> Ptr<Y> as() const;

		template<typename Y> friend class Ptr;
	};

	// Template Implementation

	template <typename T>
	void deleteArray(T * p)
	{
		delete[] p;
	}

	template <typename T>
	void deleteObject(T * p)
	{
		delete p;
	}

	class bad_dereference_exception : public std::exception
	{
	public:
		char const * what() const override
		{
			return "Bad pointer exception. ";
		}
	};

	// Needs to be a separate class (not an inner class) because otherwise it would be templated and not compatible with other templates.
	class _PtrCounter
	{
	public:
		virtual intptr_t hash() = 0; // Used in comparisons and intptr_t casting. Needed because of virtual inheritance.
		virtual void destroy() = 0;

		int pc = 1; // OwnPtr and Ptr reference counter
	};

	// This is used for proper destruction of subclasses of T.
	template <typename T>
	class _PtrCounterTyped : public _PtrCounter
	{
	public:
		_PtrCounterTyped(T * p, void(*deleteFunction) (T *))
		{
			this->p = p;
			this->deleteFunction = deleteFunction;
		}

		intptr_t hash() override
		{
			return (intptr_t)p;
		}

		void destroy() override
		{
			if (deleteFunction)
			{
				deleteFunction(p);
			}
			deleteFunction = deleteObject;
			p = nullptr;
		}

		T * p = nullptr; // Derived type for correct destruction, even without base virtual destructor.
		void(*deleteFunction) (T *) = deleteObject; // User-supplied destroy function.
	};

	template <typename T>
	PtrBase<T>::PtrBase()
	{
	}

	template <typename T> template <typename Y>
	PtrBase<T>::PtrBase(Y * p, _PtrCounter * c)
		: p(p), c(c)
	{
	}

	template <typename T>
	bool PtrBase<T>::isValid() const
	{
		return p != nullptr;
	}

	template <typename T>
	T * PtrBase<T>::operator -> () const
	{
		if (p == nullptr)
		{
			throw bad_dereference_exception(); // This points to null or to a destroyed object.
		}
		return p;
	}

	template <typename T>
	T & PtrBase<T>::operator [] (int index) const
	{
		if (p == nullptr)
		{
			throw bad_dereference_exception(); // This points to null or to a destroyed object.
		}
		return p[index];
	}

	template <typename T>
	T & PtrBase<T>::operator * () const
	{
		if (p == nullptr)
		{
			throw bad_dereference_exception(); // This points to null or to a destroyed object.
		}
		return *p;
	}

	template <typename T>
	T * PtrBase<T>::raw() const
	{
		return p;
	}

	template <typename T>
	PtrBase<T>::operator intptr_t() const
	{
		if (c == nullptr)
		{
			return 0; // The object was destroyed already, so just return 0.
		}
		return c->hash();
	}

	template <typename T> template <typename Y>
	bool PtrBase<T>::operator < (PtrBase<Y> const & ptr) const
	{
		return (intptr_t)*this < (intptr_t)ptr;
	}

	template <typename T> template <typename Y>
	bool PtrBase<T>::operator == (PtrBase<Y> const & ptr) const
	{
		return (intptr_t)*this == (intptr_t)ptr;
	}

	template <typename T>
	OwnPtr<T>::OwnPtr()
	{
	}

	template <typename T>
	OwnPtr<T>::OwnPtr(OwnPtr<T> && ownPtr)
		: PtrBase<T>(ownPtr.p, ownPtr.c)
	{
		ownPtr.p = nullptr;
		ownPtr.c = nullptr;
	}

	template <typename T> template <typename Y>
	OwnPtr<T>::OwnPtr(OwnPtr<Y> && ownPtr)
		: PtrBase<T>(ownPtr.p, ownPtr.c)
	{
		ownPtr.p = nullptr;
		ownPtr.c = nullptr;
	}

	template <typename T>
	OwnPtr<T> & OwnPtr<T>::operator = (OwnPtr<T> && ownPtr)
	{
		p = ownPtr.p;
		c = ownPtr.c;
		ownPtr.p = nullptr;
		ownPtr.c = nullptr;
		return *this;
	}

	template <typename T> template <typename Y>
	OwnPtr<T> & OwnPtr<T>::operator = (OwnPtr<Y> && ownPtr)
	{
		p = ownPtr.p;
		c = ownPtr.c;
		ownPtr.p = nullptr;
		ownPtr.c = nullptr;
		return *this;
	}

	template <typename T>
	OwnPtr<T>::~OwnPtr()
	{
		setNull();
	}

	template <typename T>
	int OwnPtr<T>::numPtrs() const
	{
		return (c != nullptr) ? (c->pc - 1) : 0;
	}

	template <typename T>
	void OwnPtr<T>::setNull()
	{
		if (p != nullptr)
		{
			c->destroy();
			c->pc--;
			if (c->pc == 0) // there is nothing else pointing to this object, so delete the counter object.
			{
				delete c;
			}
			p = nullptr;
			c = nullptr;
		}
	}

	template <typename T> template <typename Y>
	void OwnPtr<T>::setRaw(Y * newP, void(*deleteFunction) (Y *) = deleteObject)
	{
		setNull();
		p = newP;
		if (p != nullptr)
		{
			c = new _PtrCounterTyped<Y>(newP, deleteFunction);
		}
		else
		{
			c = nullptr;
		}
	}

	template <typename T> template <typename ... Args>
	void OwnPtr<T>::setNew(Args && ... args)
	{
		setRaw(new T(std::forward<Args>(args)...));
	}

	template <typename T> template <typename Y, typename ... Args>
	void OwnPtr<T>::setNew(Args && ... args)
	{
		setRaw(new Y(std::forward<Args>(args)...));
	}

	template <typename T> template <typename ... Args>
	OwnPtr<T> OwnPtr<T>::returnNew(Args && ... args)
	{
		OwnPtr<T> ptr;
		ptr.setNew(std::forward<Args>(args)...);
		return ptr;
	}

	template <typename T>
	Ptr<T>::Ptr()
	{

	}

	template <typename T>
	Ptr<T>::Ptr(Ptr<T> const & ptr)
		: PtrBase<T>(ptr.p, ptr.c)
	{
		if (c != nullptr)
		{
			c->pc++;
		}
	}

	template <typename T>
	Ptr<T>::Ptr(OwnPtr<T> const & ownPtr)
		: PtrBase<T>(ownPtr.p, ownPtr.c)
	{
		if (c != nullptr)
		{
			c->pc++;
		}
	}

	template <typename T> template <typename Y>
	Ptr<T>::Ptr(Ptr<Y> const & ptr)
		: PtrBase<T>(ptr.p, ptr.c)
	{
		if (c != nullptr)
		{
			c->pc++;
		}
	}

	template <typename T> template <typename Y>
	Ptr<T>::Ptr(OwnPtr<Y> const & ownPtr)
		: PtrBase<T>(ownPtr.p, ownPtr.c)
	{
		if (c != nullptr)
		{
			c->pc++;
		}
	}

	template <typename T>
	Ptr<T> & Ptr<T>::operator = (Ptr<T> const & ptr)
	{
		setNull();
		p = ptr.p;
		c = ptr.c;
		if (c != nullptr)
		{
			c->pc++;
		}
		return *this;
	}

	template <typename T>
	Ptr<T> & Ptr<T>::operator = (OwnPtr<T> const & ownPtr)
	{
		setNull();
		p = ownPtr.p;
		c = ownPtr.c;
		if (c != nullptr)
		{
			c->pc++;
		}
		return *this;
	}

	template <typename T> template <typename Y>
	Ptr<Y> & Ptr<T>::operator = (Ptr<Y> const & ptr)
	{
		setNull();
		p = ptr.p;
		c = ptr.c;
		if (c != nullptr)
		{
			c->pc++;
		}
		return *this;
	}

	template <typename T> template <typename Y>
	Ptr<Y> & Ptr<T>::operator = (OwnPtr<Y> const & ownPtr)
	{
		setNull();
		p = ownPtr.p;
		c = ownPtr.c;
		if (c != nullptr)
		{
			c->pc++;
		}
		return *this;
	}

	template <typename T>
	Ptr<T>::~Ptr()
	{
		setNull();
	}

	template <typename T>
	void Ptr<T>::setNull()
	{
		if (p != nullptr)
		{
			c->pc--;
			if (c->pc == 0) // this is the last OwnPtr or Ptr to point to this object, so delete the counter object.
			{
				delete c;
			}
			p = nullptr;
			c = nullptr;
		}
	}

	template <typename T> template <typename Y>
	Ptr<Y> Ptr<T>::as() const
	{
		Ptr<Y> ptr;
		ptr.p = dynamic_cast<Y *>(p);
		ptr.c = c;
		if (c != nullptr)
		{
			c->pc++;
		}
		return ptr;
	}
}

// For hash function template specialization.

namespace std
{
	template <typename T> struct hash<ve::OwnPtr<T>>
	{
		size_t operator()(ve::OwnPtr<T> const & p) const
		{
			return (intptr_t)p;
		}
	};

	template <typename T> struct hash<ve::Ptr<T>>
	{
		size_t operator()(ve::Ptr<T> const & p) const
		{
			return (intptr_t)p;
		}
	};
}

/*

Rationale:

The smart pointers shared_ptr and weak_ptr have flaws:

1. They have unnecessary thread safety which costs performance. Thread safety should be handled via other syncrhonization methods and not be in smart pointers. The underlying objects aren't thread safe even if the smart pointers are.
2. weak_ptr calls lock, which creates a shared_ptr. This is required for thread safety, but isn't useful otherwise. A weak_ptr should never be able to control the lifetime of an object owned by a shared_ptr.
3. shared_ptr has no way of knowing how many weak_ptrs reference it, so a storage system isn't possible without additional reference counting.
4. shared_ptr is copyable, but ownership of an object should be unique. This is why most say use a unique_ptr instead of shared_ptr. But a unique_ptr doesn't allow for weak_ptr references.

These OwnPtr and Ptr address these flaws:

1. They do not have thread safety. Any thread safety should be handled via other synchronization methods.
2. Ptr cannot ever extend the lifetime of OwnPtr. If Ptr references an obselete OwnPtr, an exception is thrown.
3. OwnPtr can return how many Ptrs reference it.
4. OwnPtr is unique, making ownership very clear.

*/

