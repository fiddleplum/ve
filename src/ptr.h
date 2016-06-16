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

	// An exception for accessing objects that don't exist (null pointers, bad pointers, etc).
	class nullptr_exception : public std::exception
	{
	public:
		char const * what() const override
		{
			return "Null pointer exception. ";
		}
	};

	template <typename T, bool OWN, bool USE>
	class PtrBase
	{
	public:
		// Default constructor. Initializes the pointer to null.
		PtrBase();

		// Default copy constructor. Needed otherwise C++ will create its own.
		PtrBase(PtrBase<T, OWN, USE> const & ptr);

		// Templated copy constructor. It can take a pointer that has a type that is a subclass of T.
		template <typename Y, bool OWNY, bool USEY> PtrBase(PtrBase<Y, OWNY, USEY> const & ptr);

		// Destructor. If OWN is true and this is the last reference to the object, either delete is called or the destroy function is called if it is specified. There must be no USE pointers pointing to the object.
		~PtrBase();

		// Default assignment operator. Needed otherwise C++ will create its own.
		PtrBase<T, OWN, USE> & operator = (PtrBase<T, OWN, USE> const & ptr);

		// Templated assignment operator. It can take a pointer that has a type that is a subclass of T.
		template <typename Y, bool OWNY, bool USEY> PtrBase<T, OWN, USE> & operator = (PtrBase<Y, OWNY, USEY> const & ptr);

		// Returns true if this points to something non-null.
		bool isValid() const;

		// Returns true if there is at least one PtrBase<USE = true> that points to the object this points to.
		bool isInUse() const;

		// Point the pointer to newP, which can have a type that is subclass of T. Only pass in something that looks like 'new T()' to ensure that the raw pointer isn't used elsewhere. If there was a previous object pointed to, the same algorithm as the destructor is called.
		template <typename Y> void setRaw(Y * newP, void(*deleteFunction) (Y *) = nullptr);

		// Change the object to a new pointer to an object of type T with arguments. Uses the new operator for allocation. For special allocation, use the function OwnPtr(Y * newP, ...).
		template <typename... Args> void setNew(Args... args);

		// Change the object to a new pointer to an object of type Y with arguments. Uses the new operator for allocation. For special allocation, use the function OwnPtr(Y * newP, ...).
		template <typename Y, typename... Args> void setNew(Args... args);

		// Returns a newly created OwnPtr. Same as if this were used: OwnPtr<T> ptr; ptr.setNew(args...); return ptr;
		template <typename ...Args> static PtrBase<T, OWN, USE> createNew(Args... args);

		// Resets the object to point to nothing. If there was a previous object pointed to, the same algorithm as the destructor is called.
		void setNull();

		// Provides access to the object's members.
		T * operator -> () const;

		// Provides access to the element located at index. Warning: this provides no index out-of-bounds checking.
		T & operator [] (int index) const;

		// Provides reference access to the object.
		T & operator * () const;

		// Only to be used for functions that require a raw pointer to the object. Be careful how you use this.
		T * raw() const;

		// Get the unsigned integer value of the address of the object.
		operator intptr_t() const;

		// Returns a use pointer dynamically casted to Y.
		template <typename Y> PtrBase<Y, OWN, USE> as() const;

		// Returns true if the address of this object is less than the address of ptr's object.
		template <typename Y, bool OWNY, bool USEY> bool operator < (PtrBase<Y, OWNY, USEY> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
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
	using RawPtr = PtrBase<T, false, false>;

	// Template Implementation.

	class _PtrCounter
	{
	public:
		virtual void destroy() = 0;

		int oc = 0; // PtrBase[OWN=true] reference counter
		int uc = 0; // PtrBase[USE=true] reference counter
		int ac = 0; // PtrBase[ANY] reference counter
	};

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
			else
			{
				delete p;
			}
			deleteFunction = nullptr;
			p = nullptr;
		}

		T * p = 0; // Derived type for correct destruction, even without base virtual destructor.
		void(*deleteFunction) (T *) = 0; // User-supplied destroy function.
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
			c->ac++;
		}
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	PtrBase<T, OWN, USE>::PtrBase(PtrBase<Y, OWNY, USEY> const & ptr) : p(ptr.p), c(ptr.c)
	{
		static_assert(!OWN || OWNY, "Can't convert from UsePtr to OwnPtr");
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
			c->ac++;
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
			c->ac++;
		}
		return *this;
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	PtrBase<T, OWN, USE> & PtrBase<T, OWN, USE>::operator = (PtrBase<Y, OWNY, USEY> const & ptr)
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
			c->ac++;
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
			c->ac++;
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
	PtrBase<T, OWN, USE> PtrBase<T, OWN, USE>::createNew(Args... args)
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
					throw std::exception(); // This PtrBase still has PtrBases[USE=true] out there, so it can't be deleted.
				}
				c->oc--;
				if (c->oc == 0)
				{
					c->destroy();
				}
			}
			if (USE)
			{
				c->uc--;
			}
			c->ac--;
			if (c->ac == 0) // If there are no PtrBase[ANY] around, that means this the last to reference the Counter.
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
		if (p == nullptr || c->oc == 0)
		{
			throw nullptr_exception();
		}
		return p;
	}

	template <typename T, bool OWN, bool USE>
	T & PtrBase<T, OWN, USE>::operator [] (int index) const
	{
		if (p == nullptr || c->oc == 0)
		{
			throw nullptr_exception();
		}
		return p[index];
	}

	template <typename T, bool OWN, bool USE>
	T & PtrBase<T, OWN, USE>::operator * () const
	{
		if (p == nullptr || c->oc == 0)
		{
			throw nullptr_exception();
		}
		return *p;
	}

	template <typename T, bool OWN, bool USE>
	T * PtrBase<T, OWN, USE>::raw() const
	{
		return p;
	}

	template <typename T, bool OWN, bool USE>
	PtrBase<T, OWN, USE>::operator intptr_t() const
	{
		return (intptr_t)p;
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
			c->ac++;
		}
		return pp;
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	bool PtrBase<T, OWN, USE>::operator < (PtrBase<Y, OWNY, USEY> const & ptr) const
	{
		return (void const *)p < (void const *)ptr.p;
	}

	template <typename T, bool OWN, bool USE> template <typename Y, bool OWNY, bool USEY>
	bool PtrBase<T, OWN, USE>::operator == (PtrBase<Y, OWNY, USEY> const & ptr) const
	{
		return (void const *)p == (void const *)ptr.p;
	}
}

