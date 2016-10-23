#pragma once

#include <list>
#include <unordered_set>
#include <functional>

namespace ve
{
	template <typename T>
	class ObjectList
	{
	public:
		// Default constructor.
		ObjectList();

		// Adds an item at the end.
		void push_back(T const & item);

		// Inserts value before pos
		typename std::list<T>::iterator insert(typename std::list<T>::const_iterator pos, T const & value);

		// Removes the element at pos.
		void erase(typename std::list<T>::const_iterator pos);

		// Returns true if there there are no elements.
		bool empty() const;

		// Removes all items.
		void clear();

		// Processes any removes that have accumulated since the last call.
		void processElementsToErase();

		// The iterators.
		typename std::list<T>::iterator begin();
		typename std::list<T>::iterator end();
		typename std::list<T>::const_iterator begin() const;
		typename std::list<T>::const_iterator end() const;
		typename std::list<T>::reverse_iterator rbegin();
		typename std::list<T>::reverse_iterator rend();
		typename std::list<T>::const_reverse_iterator rbegin() const;
		typename std::list<T>::const_reverse_iterator rend() const;

	private:
		class IteratorHash;
		std::list<T> list;
		std::unordered_set<typename std::list<T>::const_iterator, IteratorHash> elementsToErase;
	};

	// Internal Implementation

	template <typename T>
	ObjectList<T>::ObjectList()
	{
	}

	template <typename T>
	void ObjectList<T>::push_back(T const & item)
	{
		list.push_back(item);
	}

	template <typename T>
	typename std::list<T>::iterator ObjectList<T>::insert(typename std::list<T>::const_iterator pos, T const & value)
	{
		return list.insert(pos, value);
	}

	template <typename T>
	void ObjectList<T>::erase(typename std::list<T>::const_iterator pos)
	{
		auto result = elementsToErase.insert(pos);

	}

	template <typename T>
	bool ObjectList<T>::empty() const
	{
		return list.empty();
	}

	template <typename T>
	void ObjectList<T>::clear()
	{
		elementsToErase.clear();
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			elementsToErase.insert(it);
		}
	}

	template <typename T>
	void ObjectList<T>::processElementsToErase()
	{
		for (auto it : elementsToErase)
		{
			list.erase(it);
		}
		elementsToErase.clear();
	}

	template <typename T>
	typename std::list<T>::iterator ObjectList<T>::begin()
	{
		return list.begin();
	}

	template <typename T>
	typename std::list<T>::iterator ObjectList<T>::end()
	{
		return list.end();
	}

	template <typename T>
	typename std::list<T>::const_iterator ObjectList<T>::begin() const
	{
		return list.cbegin();
	}

	template <typename T>
	typename std::list<T>::const_iterator ObjectList<T>::end() const
	{
		return list.cend();
	}

	template <typename T>
	typename std::list<T>::reverse_iterator ObjectList<T>::rbegin()
	{
		return list.rbegin();
	}

	template <typename T>
	typename std::list<T>::reverse_iterator ObjectList<T>::rend()
	{
		return list.rend();
	}

	template <typename T>
	typename std::list<T>::const_reverse_iterator ObjectList<T>::rbegin() const
	{
		return list.crbegin();
	}

	template <typename T>
	typename std::list<T>::const_reverse_iterator ObjectList<T>::rend() const
	{
		return list.crend();
	}

	template <typename T>
	class ObjectList<T>::IteratorHash
	{
	public:
		size_t operator () (typename std::list<T>::const_iterator const & it) const
		{
			return *it;
		}
	};
}