#pragma once

#include <vector>
#include <unordered_set>
#include <functional>

namespace ve
{
	template <typename T>
	class ObjectVector
	{
	public:
		// Default constructor.
		ObjectVector();

		// Adds an element at the end.
		void push_back(T const & element);

		// Inserts an element before position
		typename std::vector<T>::iterator insert(typename std::vector<T>::const_iterator position, T const & element);

		// Removes the element at position.
		void erase(typename std::vector<T>::const_iterator position);

		// Returns true if there there are no elements.
		bool empty() const;

		// Removes all elements.
		void clear();

		// Processes any erased elements that have accumulated since the last call.
		void processElementsToErase();

		// The iterators.
		typename std::vector<T>::iterator begin();
		typename std::vector<T>::iterator end();
		typename std::vector<T>::const_iterator begin() const;
		typename std::vector<T>::const_iterator end() const;
		typename std::vector<T>::reverse_iterator rbegin();
		typename std::vector<T>::reverse_iterator rend();
		typename std::vector<T>::const_reverse_iterator rbegin() const;
		typename std::vector<T>::const_reverse_iterator rend() const;

	private:
		struct IteratorHash
		{
			size_t operator()(typename std::vector<T>::const_iterator const & it) const
			{
				return (size_t)&*it;
			}
		};

		std::vector<T> vector;
		std::unordered_set<typename std::vector<T>::const_iterator, IteratorHash> elementsToErase;
	};

	// Internal Implementation

	template <typename T>
	ObjectVector<T>::ObjectVector()
	{
	}

	template <typename T>
	void ObjectVector<T>::push_back(T const & element)
	{
		vector.push_back(element);
	}

	template <typename T>
	typename std::vector<T>::iterator ObjectVector<T>::insert(typename std::vector<T>::const_iterator position, T const & element)
	{
		return vector.insert(position, element);
	}

	template <typename T>
	void ObjectVector<T>::erase(typename std::vector<T>::const_iterator position)
	{
		elementsToErase.insert(position);
	}

	template <typename T>
	bool ObjectVector<T>::empty() const
	{
		return vector.empty();
	}

	template <typename T>
	void ObjectVector<T>::clear()
	{
		elementsToErase.clear();
		for (auto it = vector.begin(); it != vector.end(); ++it)
		{
			elementsToErase.insert(it);
		}
	}

	template <typename T>
	void ObjectVector<T>::processElementsToErase()
	{
		for (auto it : elementsToErase)
		{
			vector.erase(it);
		}
		elementsToErase.clear();
	}

	template <typename T>
	typename std::vector<T>::iterator ObjectVector<T>::begin()
	{
		return vector.begin();
	}

	template <typename T>
	typename std::vector<T>::iterator ObjectVector<T>::end()
	{
		return vector.end();
	}

	template <typename T>
	typename std::vector<T>::const_iterator ObjectVector<T>::begin() const
	{
		return vector.cbegin();
	}

	template <typename T>
	typename std::vector<T>::const_iterator ObjectVector<T>::end() const
	{
		return vector.cend();
	}

	template <typename T>
	typename std::vector<T>::reverse_iterator ObjectVector<T>::rbegin()
	{
		return vector.rbegin();
	}

	template <typename T>
	typename std::vector<T>::reverse_iterator ObjectVector<T>::rend()
	{
		return vector.rend();
	}

	template <typename T>
	typename std::vector<T>::const_reverse_iterator ObjectVector<T>::rbegin() const
	{
		return vector.crbegin();
	}

	template <typename T>
	typename std::vector<T>::const_reverse_iterator ObjectVector<T>::rend() const
	{
		return vector.crend();
	}
}