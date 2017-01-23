#pragma once

#include "ptr.hpp"
#include <map>
#include <set>
#include <functional>

namespace ve
{
	template <typename T>
	class PtrSet
	{
	public:
		// The iterators.
		class iterator;
		class const_iterator;

		// Default constructor.
		PtrSet();

		// Inserts the element. Returns the position and true, or the end and false if the element was already there.
		template <typename ... Args>
		iterator insertNew(Args && ... args);

		// Removes the element.
		void queueForErase(Ptr<T> const & ptr);

		// Returns the iterator of a given element, or end if it is not found.
		iterator find(Ptr<T> const & ptr) const;

		// Returns true if there there are no elements.
		bool empty() const;

		// Removes all elements.
		void queueAllForErase();

		// Returns true if there are no elements queued for erasure.
		bool eraseQueueIsEmpty() const;

		// Processes any erased elements that have accumulated since the last call.
		void processEraseQueue();

		// Iterators.
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

	private:
		class Hash;
		std::set<OwnPtr<T>> set;
		std::map<Ptr<T>, typename std::set<OwnPtr<T>>::const_iterator> lookup;
		std::set<Ptr<T>> eraseQueue;
	};

	//  Implementation

	template <typename T>
	class PtrSet<T>::iterator
	{
	public:
		iterator(typename std::set<OwnPtr<T>>::iterator const & iter_) : iter(iter_) {}
		iterator operator ++ () { ++iter; return *this; }
		iterator operator ++ (int junk) { iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::set<OwnPtr<T>>::iterator iter;
		friend class const_iterator;
	};

	template <typename T>
	class PtrSet<T>::const_iterator
	{
	public:
		const_iterator(typename std::set<OwnPtr<T>>::const_iterator const & iter_) : iter(iter_) {}
		const_iterator(iterator const & iter) : iter(iter.iter) {}
		const_iterator operator ++ () { ++iter; return *this; }
		const_iterator operator ++ (int junk) { const_iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (const_iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (const_iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::set<OwnPtr<T>>::const_iterator iter;
	};

	template <typename T>
	PtrSet<T>::PtrSet()
	{
	}

	template <typename T> template <typename ... Args>
	typename PtrSet<T>::iterator PtrSet<T>::insertNew(Args && ... args)
	{
		auto t = OwnPtr<T>::returnNew(args...);
		auto iter = set.insert(t);
		lookup[t] = iter.first;
		return iterator(iter.first);
	}

	template <typename T>
	void PtrSet<T>::queueForErase(Ptr<T> const & ptr)
	{
		eraseQueue.insert(ptr);
	}

	template <typename T>
	typename PtrSet<T>::iterator PtrSet<T>::find(Ptr<T> const & ptr) const
	{
		return iterator(lookup.find(ptr));
	}

	template <typename T>
	bool PtrSet<T>::empty() const
	{
		return set.empty();
	}

	template <typename T>
	void PtrSet<T>::queueAllForErase()
	{
		eraseQueue.clear();
		for (auto it = set.begin(); it != set.end(); ++it)
		{
			eraseQueue.insert(*it);
		}
	}

	template <typename T>
	bool PtrSet<T>::eraseQueueIsEmpty() const
	{
		return eraseQueue.empty();
	}

	template <typename T>
	void PtrSet<T>::processEraseQueue()
	{
		for (auto ptr : eraseQueue)
		{
			auto it = lookup.find(ptr);
			if (it != lookup.end())
			{
				set.erase(it->second);
			}
			lookup.erase(ptr);
		}
		eraseQueue.clear();
	}

	template <typename T>
	typename PtrSet<T>::iterator PtrSet<T>::begin()
	{
		return iterator(set.begin());
	}

	template <typename T>
	typename PtrSet<T>::iterator PtrSet<T>::end()
	{
		return iterator(set.end());
	}

	template <typename T>
	typename PtrSet<T>::const_iterator PtrSet<T>::begin() const
	{
		return const_iterator(set.cbegin());
	}

	template <typename T>
	typename PtrSet<T>::const_iterator PtrSet<T>::end() const
	{
		return const_iterator(set.cend());
	}

	template <typename T>
	class PtrSet<T>::Hash
	{
	public:
		size_t operator () (T const & t) const
		{
			return (size_t)t;
		}
	};
}