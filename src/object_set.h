#pragma once

#include <map>
#include <unordered_set>
#include <functional>

namespace ve
{
	template <typename T>
	class ObjectSet
	{
	public:
		// Hash used for the set. T needs to have implicit conversion to size_t.
		class Hash;

		// Default constructor.
		ObjectSet();

		// Inserts the element. Returns the position and true, or the end and false if the element was already there.
		std::pair<typename std::unordered_set<T, Hash>::iterator, bool> insert(T const & element);

		// Removes the value at the position.
		void erase(typename std::unordered_set<T, Hash>::const_iterator position);

		// Returns true if there there are no elements.
		bool empty() const;

		// Removes all elements.
		void clear();

		// Processes any removed elements that have accumulated since the last call.
		void processElementsToErase();

		// The iterators.
		typename std::unordered_set<T, Hash>::iterator begin();
		typename std::unordered_set<T, Hash>::iterator end();
		typename std::unordered_set<T, Hash>::const_iterator begin() const;
		typename std::unordered_set<T, Hash>::const_iterator end() const;

	private:
		class IteratorHash;
		std::unordered_set<T, Hash> set;
		std::unordered_set<typename std::unordered_set<T, Hash>::const_iterator, IteratorHash> elementsToErase;
	};

	// Internal Implementation

	template <typename T>
	ObjectSet<T>::ObjectSet()
	{
	}

	template <typename T>
	std::pair<typename std::unordered_set<T, typename ObjectSet<T>::Hash>::iterator, bool> ObjectSet<T>::insert(T const & element)
	{
		return set.insert(element);
	}

	template <typename T>
	void ObjectSet<T>::erase(typename std::unordered_set<T, Hash>::const_iterator position)
	{
		elementsToErase.insert(position);
	}

	template <typename T>
	bool ObjectSet<T>::empty() const
	{
		return set.empty();
	}

	template <typename T>
	void ObjectSet<T>::clear()
	{
		elementsToErase.clear();
		for (auto it = set.begin(); it != set.end(); ++it)
		{
			elementsToErase.insert(it);
		}
	}

	template <typename T>
	void ObjectSet<T>::processElementsToErase()
	{
		for (auto it : elementsToErase)
		{
			set.erase(it);
		}
		elementsToErase.clear();
	}

	template <typename T>
	typename std::unordered_set<T, typename ObjectSet<T>::Hash>::iterator ObjectSet<T>::begin()
	{
		return set.begin();
	}

	template <typename T>
	typename std::unordered_set<T, typename ObjectSet<T>::Hash>::iterator ObjectSet<T>::end()
	{
		return set.end();
	}

	template <typename T>
	typename std::unordered_set<T, typename ObjectSet<T>::Hash>::const_iterator ObjectSet<T>::begin() const
	{
		return set.cbegin();
	}

	template <typename T>
	typename std::unordered_set<T, typename ObjectSet<T>::Hash>::const_iterator ObjectSet<T>::end() const
	{
		return set.cend();
	}

	template <typename T>
	class ObjectSet<T>::IteratorHash
	{
	public:
		size_t operator () (typename std::unordered_set<T, Hash>::const_iterator const & it) const
		{
			return *it;
		}
	};

	template <typename T>
	class ObjectSet<T>::Hash
	{
	public:
		size_t operator () (T const & t) const
		{
			return (std::string)t;
		}
	};
}