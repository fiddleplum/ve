#pragma once

#include <exception>

namespace ve
{
	// Forward declaration
	class _PtrCounter;

	// This is a function that can be passed to OwnPtr to delete arrays properly.
	template <typename T>
	void deleteArray(T * p)
	{
		delete[] p;
	}

	// This is the standard delete function. It just calls the delete operator.
	template <typename T>
	void deleteObject(T * p)
	{
		delete p;
	}

	// An exception for accessing objects that don't exist (null pointers, bad pointers, etc).
	class nullptr_exception : public std::exception
	{
	public:
		char const * what() const override
		{
			return "Null pointer exception. ";
		}
	};

	// An exception for when an OwnPtr destructs but there are still UsePtrs left.
	class bad_destroy_exception : public std::exception
	{
	public:
		char const * what() const override
		{
			return "Bad OwnPtr destruction exception. ";
		}
	};

	// The base class for OwnPtr and UsePtr.
	template <typename T, bool OWN>
	class PtrBase final
	{
	public:
		// Default constructor. Initializes this to point to null.
		PtrBase();

		// Default copy constructor. Needed otherwise C++ will create its own.
		PtrBase(PtrBase<T, OWN> const & ptr);

		// Templated copy constructor. It can take a PtrBase that has a type that is a subclass of T. An OwnPtr cannot be copy-constructed from a UsePtr.
		template <typename Y, bool OWNY> PtrBase(PtrBase<Y, OWNY> const & ptr);

		// Destructor. If this is an OwnPtr and is the last reference to the object, either delete is called or the destroy function is called if it is specified. There must be no UsePtrs pointing to the object.
		~PtrBase();

		// Default assignment operator. Needed otherwise C++ will create its own.
		PtrBase<T, OWN> & operator = (PtrBase<T, OWN> const & ptr);

		// Templated assignment operator. This can take a BasePtr that has a type that is a subclass of T. A UsePtr cannot be assigned to an OwnPtr.
		template <typename Y, bool OWNY> PtrBase<T, OWN> & operator = (PtrBase<Y, OWNY> const & ptr);

		// Returns true if this points to something non-null.
		bool isValid() const;

		// Returns true if there is at least one UsePtr that points to the object this points to.
		bool isInUse() const;

		// Point the pointer to newP, which can have a type that is subclass of T. Only pass in something that looks like 'new T()' to ensure that the raw pointer isn't used elsewhere.
		template <typename Y> void setRaw(Y * newP, void(*deleteFunction) (Y *) = deleteObject);

		// Change the object to a new pointer to an object of type T with arguments. Uses the new operator for allocation. For special allocation use the function setRaw().
		template <typename... Args> void setNew(Args... args);

		// Change the object to a new pointer to an object of type Y with arguments. Uses the new operator for allocation. For special allocation the function setRaw().
		template <typename Y, typename... Args> void setNew(Args... args);

		// Returns a newly created OwnPtr using setNew above.
		template <typename ...Args> static PtrBase<T, OWN> createNew(Args... args);

		// Resets this to point to null.
		void setNull();

		// Provides access to the pointed-to object's members.
		T * operator -> () const;

		// Provides access to the element located at index. Warning: this provides no index out-of-bounds checking.
		T & operator [] (int index) const;

		// Provides reference access to the pointed-to object's members.
		T & operator * () const;

		// Only to be used for functions that require a raw pointer to the object. Be careful how you use this.
		T * raw() const;

		// Get the unsigned integer value of the address of the pointed-to object.
		operator intptr_t() const;

		// Returns a PtrBase dynamically casted to Y.
		template <typename Y> PtrBase<Y, OWN> as() const;

		// Returns true if the address of this's object is less than the address of ptr's object.
		template <typename Y, bool OWNY> bool operator < (PtrBase<Y, OWNY> const & ptr) const;

		// Returns true if the address of this's object is equal to the address of ptr's object.
		template <typename Y, bool OWNY> bool operator == (PtrBase<Y, OWNY> const & ptr) const;

	private:
		T * p;
		_PtrCounter * c;

		template<typename Y, bool OWNY> friend class PtrBase;
	};

	template <typename T>
	using OwnPtr = PtrBase<T, true>;

	template <typename T>
	using UsePtr = PtrBase<T, false>;

	// Template Implementation.

	// Needs to be a separate class (not an inner class) because otherwise it would be templated and not compatible with other templates.
	class _PtrCounter
	{
	public:
		virtual void destroy() = 0;

		int oc = 0; // PtrBase[OWN=true] reference counter
		int uc = 0; // PtrBase[USE=true] reference counter
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

	template <typename T, bool OWN>
	PtrBase<T, OWN>::PtrBase() : p(nullptr), c(nullptr)
	{
	}

	template <typename T, bool OWN>
	PtrBase<T, OWN>::PtrBase(PtrBase<T, OWN> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			else
			{
				c->uc++;
			}
		}
	}

	template <typename T, bool OWN> template <typename Y, bool OWNY>
	PtrBase<T, OWN>::PtrBase(PtrBase<Y, OWNY> const & ptr) : p(ptr.p), c(ptr.c)
	{
		static_assert(!OWN || OWNY, "Can't convert from UsePtr to OwnPtr");
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			else
			{
				c->uc++;
			}
		}
	}

	template <typename T, bool OWN>
	PtrBase<T, OWN>::~PtrBase()
	{
		setNull();
	}

	template <typename T, bool OWN>
	PtrBase<T, OWN> & PtrBase<T, OWN>::operator = (PtrBase<T, OWN> const & ptr)
	{
		setNull();
		p = ptr.p;
		c = ptr.c;
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			else
			{
				c->uc++;
			}
		}
		return *this;
	}

	template <typename T, bool OWN> template <typename Y, bool OWNY>
	PtrBase<T, OWN> & PtrBase<T, OWN>::operator = (PtrBase<Y, OWNY> const & ptr)
	{
		setNull();
		p = ptr.p;
		c = ptr.c;
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			else
			{
				c->uc++;
			}
		}
		return *this;
	}

	template <typename T, bool OWN>
	bool PtrBase<T, OWN>::isValid() const
	{
		return p != nullptr;
	}

	template <typename T, bool OWN>
	bool PtrBase<T, OWN>::isInUse() const
	{
		return p != nullptr && c->uc != 0;
	}

	template <typename T, bool OWN> template <typename Y>
	void PtrBase<T, OWN>::setRaw(Y * newP, void(*deleteFunction) (Y *))
	{
		if (!OWN)
		{
			static_assert(OWN, "Must be OwnPtr");
		}
		setNull();
		p = newP;
		if (p != nullptr)
		{
			c = new _PtrCounterTyped<Y>(newP, deleteFunction);
			c->oc++;
		}
		else
		{
			c = nullptr;
		}
	}

	template <typename T, bool OWN> template <typename... Args>
	void PtrBase<T, OWN>::setNew(Args... args)
	{
		setRaw(new T(args...));
	}

	template <typename T, bool OWN> template <typename Y, typename... Args>
	void PtrBase<T, OWN>::setNew(Args... args)
	{
		setRaw(new Y(args...));
	}

	template <typename T, bool OWN> template <typename ...Args>
	PtrBase<T, OWN> PtrBase<T, OWN>::createNew(Args... args)
	{
		PtrBase<T, OWN> ptr;
		ptr.setNew(args...);
		return ptr;
	}

	template <typename T, bool OWN>
	void PtrBase<T, OWN>::setNull()
	{
		if (p != nullptr)
		{
			if (OWN)
			{
				if (c->oc == 1 && c->uc > 0)
				{
					throw bad_destroy_exception(); // This OwnPtr still has other UsePtrs out there, so it can't be destroyed. All other UsePtrs need to be cleared.
				}
				c->oc--;
				if (c->oc == 0)
				{
					c->destroy();
					delete c;
				}
			}
			else
			{
				c->uc--;
			}
			p = nullptr;
			c = nullptr;
		}
	}

	template <typename T, bool OWN>
	T * PtrBase<T, OWN>::operator -> () const
	{
		if (p == nullptr || c->oc == 0)
		{
			throw nullptr_exception(); // This points to null.
		}
		return p;
	}

	template <typename T, bool OWN>
	T & PtrBase<T, OWN>::operator [] (int index) const
	{
		if (p == nullptr || c->oc == 0)
		{
			throw nullptr_exception(); // This points to null.
		}
		return p[index];
	}

	template <typename T, bool OWN>
	T & PtrBase<T, OWN>::operator * () const
	{
		if (p == nullptr || c->oc == 0)
		{
			throw nullptr_exception(); // This points to null.
		}
		return *p;
	}

	template <typename T, bool OWN>
	T * PtrBase<T, OWN>::raw() const
	{
		return p;
	}

	template <typename T, bool OWN>
	PtrBase<T, OWN>::operator intptr_t() const
	{
		return (intptr_t)p;
	}

	template <typename T, bool OWN> template <typename Y>
	PtrBase<Y, OWN> PtrBase<T, OWN>::as() const
	{
		PtrBase<Y, OWN> pp;
		pp.p = dynamic_cast<Y *>(p);
		pp.c = c;
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			else
			{
				c->uc++;
			}
		}
		return pp;
	}

	template <typename T, bool OWN> template <typename Y, bool OWNY>
	bool PtrBase<T, OWN>::operator < (PtrBase<Y, OWNY> const & ptr) const
	{
		return (void const *)p < (void const *)ptr.p;
	}

	template <typename T, bool OWN> template <typename Y, bool OWNY>
	bool PtrBase<T, OWN>::operator == (PtrBase<Y, OWNY> const & ptr) const
	{
		return (void const *)c == (void const *)ptr.c;
	}
}