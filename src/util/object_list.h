#pragma once

#include <vector>
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

		// Adds an element at the end.
		void push_back(T const & element);

		// Inserts an element before position
		typename std::list<T>::iterator insert(typename std::list<T>::const_iterator position, T const & element);

		// Adds the element into the trash.
		void queueForErase(typename std::list<T>::const_iterator position);

		// Returns true if there there are no elements.
		bool empty() const;

		// Removes all elements.
		void queueAllForErase();

		// Processes any erased elements that have accumulated since the last call.
		void processEraseQueue();

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
		struct IteratorHash
		{
			size_t operator()(typename std::list<T>::const_iterator const & it) const
			{
				return (size_t)&*it;
			}
		};

		std::list<T> list;
		std::unordered_set<typename std::list<T>::const_iterator, IteratorHash> eraseQueue;
	};

	// Internal Implementation

	template <typename T>
	ObjectList<T>::ObjectList()
	{
	}

	template <typename T>
	void ObjectList<T>::push_back(T const & element)
	{
		list.push_back(element);
	}

	template <typename T>
	typename std::list<T>::iterator ObjectList<T>::insert(typename std::list<T>::const_iterator position, T const & element)
	{
		return list.insert(position, element);
	}

	template <typename T>
	void ObjectList<T>::queueForErase(typename std::list<T>::const_iterator position)
	{
		eraseQueue.insert(position);
	}

	template <typename T>
	bool ObjectList<T>::empty() const
	{
		return list.empty();
	}

	template <typename T>
	void ObjectList<T>::queueAllForErase()
	{
		eraseQueue.clear();
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			eraseQueue.insert(it);
		}
	}

	template <typename T>
	void ObjectList<T>::processEraseQueue()
	{
		for (auto it : eraseQueue)
		{
			list.erase(it);
		}
		eraseQueue.clear();
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
}