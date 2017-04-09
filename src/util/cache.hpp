#pragma once

#include <memory>
#include <unordered_set>
#include <stdexcept>

/*
If I use Ptr,
1. Users must explicitly call create and destroy and clean cannot exist. -
2. If a user uses a destroyed ptr, the error is at the access. +
If I use UsePtr,
1. Users just stop using their UsePtr and call clean. +
2. If the Cache is destructed and there are still UsePtrs out there, it errors in the destructor. -

If I can ensure that all UsePtrs are no longer used, then UsePtrs are better.
*/

namespace ve
{
	// Generic object cache.
	template <typename T>
	class Cache
	{
	public:
		// Destructor. Calls clean(). Throws an exception if it still has any objects.
		~Cache();

		// Constructs and returns a new object.
		template <typename ... Args> UsePtr<T> create(Args && ... args);

		// Removes and destroys the objects that aren't referenced outside of the cache by UsePtrs.
		void clean();

		// The iterators.
		class iterator;
		class const_iterator;

		// Iterator accessors.
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

	private:
		std::unordered_set<OwnPtr<T>> objects;
	};

	// Template Implementations

	template <typename T>
	class Cache<T>::iterator
	{
	public:
		iterator(typename std::unordered_set<OwnPtr<T>>::iterator const & iter_) : iter(iter_) {}
		iterator operator ++ () { ++iter; return *this; }
		iterator operator ++ (int junk) { iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::unordered_set<OwnPtr<T>>::iterator iter;
		friend class const_iterator;
	};

	template <typename T>
	class Cache<T>::const_iterator
	{
	public:
		const_iterator(typename std::unordered_set<OwnPtr<T>>::const_iterator const & iter_) : iter(iter_) {}
		const_iterator(iterator const & iter) : iter(iter.iter) {}
		const_iterator operator ++ () { ++iter; return *this; }
		const_iterator operator ++ (int junk) { const_iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (const_iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (const_iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::unordered_set<OwnPtr<T>>::const_iterator iter;
	};

	template <typename T>
	Cache<T>::~Cache()
	{
		clean();
		if (!objects.empty())
		{
			//throw std::runtime_error("Cannot destruct. There are still objects referenced. ");
		}
	}

	template <typename T> template <typename ... Args>
	UsePtr<T> Cache<T>::create(Args && ... args)
	{
		OwnPtr<T> object;
		try
		{
			object.setNew(std::forward<Args>(args)...);
		}
		catch (std::runtime_error const & e)
		{
			throw std::runtime_error(std::string("Error while constructing object: ") + e.what());
		}
		objects.insert(object);
		return object;
	}

	template <typename T>
	void Cache<T>::clean()
	{
		for (auto it = objects.begin(); it != objects.end();)
		{
			if (!it->isInUse())
			{
				it = objects.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	template <typename T>
	typename Cache<T>::iterator Cache<T>::begin()
	{
		return iterator(objects.begin());
	}

	template <typename T>
	typename Cache<T>::iterator Cache<T>::end()
	{
		return iterator(objects.end());
	}

	template <typename T>
	typename Cache<T>::const_iterator Cache<T>::begin() const
	{
		return const_iterator(objects.cbegin());
	}

	template <typename T>
	typename Cache<T>::const_iterator Cache<T>::end() const
	{
		return const_iterator(objects.cend());
	}
}