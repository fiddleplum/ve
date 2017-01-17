#pragma once

#include "util/ptr.hpp"
#include <list>
#include <unordered_set>
#include <map>
#include <functional>

namespace ve
{
	template <typename T>
	class PtrList
	{
	public:
		// The iterators.
		class iterator;
		class const_iterator;
		class reverse_iterator;
		class const_reverse_iterator;

		// Default constructor.
		PtrList();

		// Adds an element at the end. Y should be a [sub] class of T.
		template <typename Y, typename ... Args>
		Ptr<Y> appendNew(Args && ... args);

		// Inserts an element before position
		template <typename Y, typename ... Args>
		Ptr<Y> insertNew(const_iterator const & position, Args && ... args);

		// Adds the element into the trash.
		void queueForErase(typename const_iterator const & position);

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
		reverse_iterator rbegin();
		reverse_iterator rend();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

	private:
		struct hash
		{
			size_t operator()(typename std::list<OwnPtr<T>>::const_iterator const & it) const
			{
				return (size_t)&*it;
			}
		};

		std::list<OwnPtr<T>> list;
		std::map<Ptr<T>, typename std::list<OwnPtr<T>>::const_iterator> lookup;
		std::unordered_set<typename std::list<OwnPtr<T>>::const_iterator, hash> eraseQueue;
	};

	//  Implementation

	template <typename T>
	class PtrList<T>::iterator
	{
	public:
		iterator(typename std::list<OwnPtr<T>>::iterator const & iter_) : iter(iter_) {}
		iterator operator ++ () { ++iter; return *this; }
		iterator operator ++ (int junk) { iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::list<OwnPtr<T>>::iterator iter;
		friend class const_iterator;
		friend class PtrList<T>;
	};

	template <typename T>
	class PtrList<T>::const_iterator
	{
	public:
		const_iterator(typename std::list<OwnPtr<T>>::const_iterator const & iter_) : iter(iter_) {}
		const_iterator(iterator const & iter) : iter(iter.iter) {}
		const_iterator operator ++ () { ++iter; return *this; }
		const_iterator operator ++ (int junk) { const_iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (const_iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (const_iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::list<OwnPtr<T>>::const_iterator iter;
		friend class PtrList<T>;
	};

	template <typename T>
	class PtrList<T>::reverse_iterator
	{
	public:
		reverse_iterator(typename std::list<OwnPtr<T>>::reverse_iterator const & iter_) : iter(iter_) {}
		reverse_iterator operator ++ () { ++iter; return *this; }
		reverse_iterator operator ++ (int junk) { reverse_iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (reverse_iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (reverse_iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::list<OwnPtr<T>>::reverse_iterator iter;
		friend class const_reverse_iterator;
		friend class PtrList<T>;
	};

	template <typename T>
	class PtrList<T>::const_reverse_iterator
	{
	public:
		const_reverse_iterator(typename std::list<OwnPtr<T>>::const_reverse_iterator const & iter_) : iter(iter_) {}
		const_reverse_iterator(reverse_iterator const & iter) : iter(iter.iter) {}
		const_reverse_iterator operator ++ () { ++iter; return *this; }
		const_reverse_iterator operator ++ (int junk) { const_reverse_iterator i = *this; iter++; return i; }
		Ptr<T> operator * () { return *iter; }
		bool operator == (const_reverse_iterator const & rhs) { return iter == rhs.iter; }
		bool operator != (const_reverse_iterator const & rhs) { return iter != rhs.iter; }

	private:
		typename std::list<OwnPtr<T>>::const_reverse_iterator iter;
		friend class PtrList<T>;
	};

	template <typename T>
	PtrList<T>::PtrList()
	{
	}

	template <typename T> template <typename Y, typename ... Args>
	typename Ptr<Y> PtrList<T>::appendNew(Args && ... args)
	{
		auto t = OwnPtr<Y>::returnNew(args...);
		auto iter = list.insert(list.end(), t);
		lookup[t] = iter;
		return t;
	}

	template <typename T> template <typename Y, typename ... Args>
	typename Ptr<Y> PtrList<T>::insertNew(const_iterator const & position, Args && ... args)
	{
		auto t = OwnPtr<Y>::returnNew(args...);
		list.insert(position, t);
		return t;
	}

	template <typename T>
	void PtrList<T>::queueForErase(const_iterator const & position)
	{
		eraseQueue.insert(position.iter);
	}

	template <typename T>
	bool PtrList<T>::empty() const
	{
		return list.empty();
	}

	template <typename T>
	void PtrList<T>::queueAllForErase()
	{
		eraseQueue.clear();
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			eraseQueue.insert(it);
		}
	}

	template <typename T>
	bool PtrList<T>::eraseQueueIsEmpty() const
	{
		return eraseQueue.empty();
	}

	template <typename T>
	void PtrList<T>::processEraseQueue()
	{
		for (auto it : eraseQueue)
		{
			list.erase(lookup.find(*it)->second);
			lookup.erase(*it);
		}
		eraseQueue.clear();
	}

	template <typename T>
	typename PtrList<T>::iterator PtrList<T>::begin()
	{
		return list.begin();
	}

	template <typename T>
	typename PtrList<T>::iterator PtrList<T>::end()
	{
		return list.end();
	}

	template <typename T>
	typename PtrList<T>::const_iterator PtrList<T>::begin() const
	{
		return list.cbegin();
	}

	template <typename T>
	typename PtrList<T>::const_iterator PtrList<T>::end() const
	{
		return list.cend();
	}

	//template <typename T>
	//typename std::list<T>::reverse_iterator PtrList<T>::rbegin()
	//{
	//	return list.rbegin();
	//}

	//template <typename T>
	//typename std::list<T>::reverse_iterator PtrList<T>::rend()
	//{
	//	return list.rend();
	//}

	//template <typename T>
	//typename std::list<T>::const_reverse_iterator PtrList<T>::rbegin() const
	//{
	//	return list.crbegin();
	//}

	//template <typename T>
	//typename std::list<T>::const_reverse_iterator PtrList<T>::rend() const
	//{
	//	return list.crend();
	//}
}