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
	class badptr_exception : public std::exception
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
	template <typename T, bool OWN, bool USE>
	class PtrBase final
	{
	public:
		// Default constructor. Initializes this to point to null.
		PtrBase();

		// Default copy constructor. Needed otherwise C++ will create its own.
		PtrBase(PtrBase<T, OWN, USE> const & ptr);

		// Templated copy constructor. It can take a PtrBase that has a type that is a subclass of T. An OwnPtr cannot be copy-constructed from a UsePtr.
		template <typename Y, bool OWNY, bool USEY> PtrBase(PtrBase<Y, OWNY, USEY> const & ptr);

		// Destructor. If this is an OwnPtr and is the last reference to the object, either delete is called or the destroy function is called if it is specified. There must be no UsePtrs pointing to the object.
		~PtrBase();

		// Default assignment operator. Needed otherwise C++ will create its own.
		PtrBase<T, OWN, USE> & operator = (PtrBase<T, OWN, USE> const & ptr);

		// Templated assignment operator. This can take a BasePtr that has a type that is a subclass of T. A UsePtr cannot be assigned to an OwnPtr.
		template <typename Y, bool OWNY, bool USEY> PtrBase<T, OWN, USE> & operator = (PtrBase<Y, OWNY, USEY> const & ptr);

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
		template <typename ...Args> static PtrBase<T, OWN, USE> returnNew(Args... args);

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
		template <typename Y> PtrBase<Y, OWN, USE> as() const;

		// Returns true if the address of this's object is less than the address of ptr's object.
		template <typename Y, bool OWNY, bool USEY> bool operator < (PtrBase<Y, OWNY, USEY> const & ptr) const;

		// Returns true if the address of this's object is equal to the address of ptr's object.
		template <typename Y, bool OWNY, bool USEY> bool operator == (PtrBase<Y, OWNY, USEY> const & ptr) const;

	private:
		T * p;
		_PtrCounter * c;

		template<typename Y, bool OWNY, bool USEY> friend class PtrBase;
	};

	template <typename T>
	using OwnPtr = PtrBase<T, true, false>;

	template <typename T>
	using UsePtr = PtrBase<T, false, true>;

	template <typename T>
	using Ptr = PtrBase<T, false, false>;

	// Template Implementation.

	// Needs to be a separate class (not an inner class) because otherwise it would be templated and not compatible with other templates.
	class _PtrCounter
	{
	public:
		virtual intptr_t hash() = 0; // Used in comparisons and intptr_t casting. Needed because of virtual inheritance.
		virtual void destroy() = 0;

		int oc = 0; // PtrBase[OWN=true] reference counter
		int uc = 0; // PtrBase[USE=true] reference counter
		int pc = 0; // PtrBase[ANY] reference counter
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
			if (oc == 0)
			{
				return 0;
			}
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

	template <typename T, bool OWN, bool USE>
	PtrBase<T, OWN, USE>::PtrBase() : p(nullptr), c(nullptr)
	{
	}

	template <typename T, bool OWN, bool USE>
	PtrBase<T, OWN, USE>::PtrBase(PtrBase<T, OWN, USE> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			if (USE)
			{
				c->uc++;
			}
			c->pc++;
		}
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	PtrBase<T, OWN, USE>::PtrBase(PtrBase<Y, OWNY, USEY> const & ptr) : p(ptr.p), c(ptr.c)
	{
		static_assert(!OWN || OWNY, "Can't convert from Ptr or UsePtr to OwnPtr");
		static_assert(!USE || USEY || OWNY, "Can't convert from Ptr to UsePtr");
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			if (USE)
			{
				c->uc++;
			}
			c->pc++;
		}
	}

	template <typename T, bool OWN, bool USE>
	PtrBase<T, OWN, USE>::~PtrBase()
	{
		setNull();
	}

	template <typename T, bool OWN, bool USE>
	PtrBase<T, OWN, USE> & PtrBase<T, OWN, USE>::operator = (PtrBase<T, OWN, USE> const & ptr)
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
			if (USE)
			{
				c->uc++;
			}
			c->pc++;
		}
		return *this;
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	PtrBase<T, OWN, USE> & PtrBase<T, OWN, USE>::operator = (PtrBase<Y, OWNY, USEY> const & ptr)
	{
		static_assert(!OWN || OWNY, "Can't convert from Ptr or UsePtr to OwnPtr");
		static_assert(!USE || USEY || OWNY, "Can't convert from Ptr to UsePtr");
		setNull();
		p = ptr.p;
		c = ptr.c;
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			if (USE)
			{
				c->uc++;
			}
			c->pc++;
		}
		return *this;
	}

	template <typename T, bool OWN, bool USE>
	bool PtrBase<T, OWN, USE>::isValid() const
	{
		return p != nullptr;
	}

	template <typename T, bool OWN, bool USE>
	bool PtrBase<T, OWN, USE>::isInUse() const
	{
		return p != nullptr && c->uc != 0;
	}

	template <typename T, bool OWN, bool USE> template <typename Y>
	void PtrBase<T, OWN, USE>::setRaw(Y * newP, void(*deleteFunction) (Y *))
	{
		static_assert(OWN, "Must be OwnPtr");
		setNull();
		p = newP;
		if (p != nullptr)
		{
			c = new _PtrCounterTyped<Y>(newP, deleteFunction);
			c->oc++;
			c->pc++;
		}
		else
		{
			c = nullptr;
		}
	}

	template <typename T, bool OWN, bool USE> template <typename... Args>
	void PtrBase<T, OWN, USE>::setNew(Args... args)
	{
		setRaw(new T(args...));
	}

	template <typename T, bool OWN, bool USE> template <typename Y, typename... Args>
	void PtrBase<T, OWN, USE>::setNew(Args... args)
	{
		setRaw(new Y(args...));
	}

	template <typename T, bool OWN, bool USE> template <typename ...Args>
	PtrBase<T, OWN, USE> PtrBase<T, OWN, USE>::returnNew(Args... args)
	{
		PtrBase<T, OWN, USE> ptr;
		ptr.setNew(args...);
		return ptr;
	}

	template <typename T, bool OWN, bool USE>
	void PtrBase<T, OWN, USE>::setNull()
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
				if (c->oc == 0) // Last OwnPtr, so destroy the object. The counter object still stays around until no other PtrBases are pointing to the now-defunct object.
				{
					c->destroy();
				}
			}
			if (USE)
			{
				c->uc--;
			}
			c->pc--;
			if (c->pc == 0) // this is the last PtrBase to point to this object, so delete the counter object.
			{
				delete c;
			}
			p = nullptr;
			c = nullptr;
		}
	}

	template <typename T, bool OWN, bool USE>
	T * PtrBase<T, OWN, USE>::operator -> () const
	{
		if (c == nullptr || c->oc == 0)
		{
			throw badptr_exception(); // This points to null or to a destroyed object.
		}
		return p;
	}

	template <typename T, bool OWN, bool USE>
	T & PtrBase<T, OWN, USE>::operator [] (int index) const
	{
		if (c == nullptr || c->oc == 0)
		{
			throw badptr_exception(); // This points to null or to a destroyed object.
		}
		return p[index];
	}

	template <typename T, bool OWN, bool USE>
	T & PtrBase<T, OWN, USE>::operator * () const
	{
		if (c == nullptr || c->oc == 0)
		{
			throw badptr_exception(); // This points to null or to a destroyed object.
		}
		return *p;
	}

	template <typename T, bool OWN, bool USE>
	T * PtrBase<T, OWN, USE>::raw() const
	{
		if (c == nullptr || c->oc == 0)
		{
			return nullptr; // The object was destroyed already, so just return a null_ptr.
		}
		return p;
	}

	template <typename T, bool OWN, bool USE>
	PtrBase<T, OWN, USE>::operator intptr_t() const
	{
		if (c == nullptr)
		{
			return 0; // The object was destroyed already, so just return 0.
		}
		return c->hash();
	}

	template <typename T, bool OWN, bool USE> template <typename Y>
	PtrBase<Y, OWN, USE> PtrBase<T, OWN, USE>::as() const
	{
		PtrBase<Y, OWN, USE> pp;
		pp.p = dynamic_cast<Y *>(p);
		pp.c = c;
		if (p != nullptr)
		{
			if (OWN)
			{
				c->oc++;
			}
			if (USE)
			{
				c->uc++;
			}
			c->pc++;
		}
		return pp;
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	bool PtrBase<T, OWN, USE>::operator < (PtrBase<Y, OWNY, USEY> const & ptr) const
	{
		return (intptr_t)*this < (intptr_t)ptr;
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	bool PtrBase<T, OWN, USE>::operator == (PtrBase<Y, OWNY, USEY> const & ptr) const
	{
		return (intptr_t)*this == (intptr_t)ptr;
	}
}

// For hash function template specialization.

namespace std
{
	template <typename T, bool OWN, bool USE> struct hash<ve::PtrBase<T, OWN, USE>>
	{
		size_t operator()(ve::PtrBase<T, OWN, USE> const & p) const
		{
			return (intptr_t)p;
		}
	};
}