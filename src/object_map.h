#pragma once

#include <map>
#include <unordered_set>
#include <functional>

namespace ve
{
	template <typename K, typename V>
	class ObjectMap
	{
	public:
		// Default constructor.
		ObjectMap();

		// Inserts value at the key. Returns the position and true, or the end and false if the value was already there.
		std::pair<typename std::map<K, V>::iterator, bool> insert(K const & key, V const & value);

		// Removes the value at the position.
		void erase(typename std::map<K, V>::const_iterator position);

		// Returns an iterator to the key, or end().
		typename std::map<K, V>::iterator find(K const & key);

		// Returns an iterator to the key, or end().
		typename std::map<K, V>::const_iterator find(K const & key) const;

		// Returns true if there there are no elements.
		bool empty() const;

		// Removes all elements.
		void clear();

		// Processes any removed elements that have accumulated since the last call.
		void processElementsToErase();

		// The iterators.
		typename std::map<K, V>::iterator begin();
		typename std::map<K, V>::iterator end();
		typename std::map<K, V>::const_iterator begin() const;
		typename std::map<K, V>::const_iterator end() const;
		typename std::map<K, V>::reverse_iterator rbegin();
		typename std::map<K, V>::reverse_iterator rend();
		typename std::map<K, V>::const_reverse_iterator rbegin() const;
		typename std::map<K, V>::const_reverse_iterator rend() const;

	private:
		class IteratorHash;
		std::map<K, V> map;
		std::unordered_set<typename std::map<K, V>::const_iterator, IteratorHash> elementsToErase;
	};

	// Internal Implementation

	template <typename K, typename V>
	ObjectMap<K, V>::ObjectMap()
	{
	}

	template <typename K, typename V>
	std::pair<typename std::map<K, V>::iterator, bool> ObjectMap<K, V>::insert(K const & key, V const & value)
	{
		return map.insert(std::pair<K const, V>(key, value));
	}

	template <typename K, typename V>
	void ObjectMap<K, V>::erase(typename std::map<K, V>::const_iterator position)
	{
		elementsToErase.insert(position);
	}

	template <typename K, typename V>
	typename std::map<K, V>::iterator ObjectMap<K, V>::find(K const & key)
	{
		return map.find(key);
	}

	template <typename K, typename V>
	typename std::map<K, V>::const_iterator ObjectMap<K, V>::find(K const & key) const
	{
		return map.find(key);
	}

	template <typename K, typename V>
	bool ObjectMap<K, V>::empty() const
	{
		return map.empty();
	}

	template <typename K, typename V>
	void ObjectMap<K, V>::clear()
	{
		elementsToErase.clear();
		for (auto it = map.begin(); it != map.end(); ++it)
		{
			elementsToErase.insert(it);
		}
	}

	template <typename K, typename V>
	void ObjectMap<K, V>::processElementsToErase()
	{
		for (auto it : elementsToErase)
		{
			map.erase(it);
		}
		elementsToErase.clear();
	}

	template <typename K, typename V>
	typename std::map<K, V>::iterator ObjectMap<K, V>::begin()
	{
		return map.begin();
	}

	template <typename K, typename V>
	typename std::map<K, V>::iterator ObjectMap<K, V>::end()
	{
		return map.end();
	}

	template <typename K, typename V>
	typename std::map<K, V>::const_iterator ObjectMap<K, V>::begin() const
	{
		return map.cbegin();
	}

	template <typename K, typename V>
	typename std::map<K, V>::const_iterator ObjectMap<K, V>::end() const
	{
		return map.cend();
	}

	template <typename K, typename V>
	typename std::map<K, V>::reverse_iterator ObjectMap<K, V>::rbegin()
	{
		return map.rbegin();
	}

	template <typename K, typename V>
	typename std::map<K, V>::reverse_iterator ObjectMap<K, V>::rend()
	{
		return map.rend();
	}

	template <typename K, typename V>
	typename std::map<K, V>::const_reverse_iterator ObjectMap<K, V>::rbegin() const
	{
		return map.crbegin();
	}

	template <typename K, typename V>
	typename std::map<K, V>::const_reverse_iterator ObjectMap<K, V>::rend() const
	{
		return map.crend();
	}

	template <typename K, typename V>
	class ObjectMap<K, V>::IteratorHash
	{
	public:
		size_t operator () (typename std::map<K, V>::const_iterator const & it) const
		{
			return *it;
		}
	};
}