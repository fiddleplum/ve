#pragma once

#include <vector>
#include <deque>

// This is an vector of items whereby the adds and removes are added to a to-do list which is processed after any looping.
template <typename T>
class ContainerVector
{
public:
	// Default constructor.
	ContainerVector();

	// Initializer list constructor.
	ContainerVector(std::initializer_list<T> il);

	// Finds an element based on another type.
	template <typename Y> T & find(Y const & item);

	// Finds an element based on another type.
	template <typename Y> T const & find(Y const & item) const;

	// Adds an item at the end.
	template <typename Y> void add(Y const & item);

	// Inserts an item before another item.
	template <typename Y> void insert(int index, Y const & item);

	// Removes an item.
	template <typename Y> T const & remove(int index);

	// Removes all items.
	void clear();

	// Processes any removes that have accumulated since the last call.
	void processRemoves();

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
	std::vector<T> items;
	std::deque<bool> commands; // bool is true=add, false=remove
	std::deque<std::pair<int, T>> adds;
	std::deque<int> removes;
};

// Internal Implementation

template <typename T>
ContainerVector<T>::ContainerVector()
{
}

template <typename T>
ContainerVector<T>::ContainerVector(std::initializer_list<T> il)
{
	for(auto const & item : il)
	{
		items.push_back(item);
	}
}

template <typename T> template <typename Y>
T & ContainerVector<T>::find(Y const & item)
{
	for(auto & t : items)
	{
		if(t == item)
		{
			return t;
		}
	}
	throw std::exception();
}

template <typename T> template <typename Y>
T const & ContainerVector<T>::find(Y const & item) const
{
	for(auto const & t : items)
	{
		if(t == item)
		{
			return t;
		}
	}
	throw std::exception();
}

template <typename T> template <typename Y>
void ContainerVector<T>::add(Y const & item)
{
	commands.push_back(true);
	adds.push_back(std::pair<int, T>(items.size(), item));
}

template <typename T> template <typename Y>
void ContainerVector<T>::insert(int index, Y const & item)
{
	commands.push_back(true);
	adds.push_back(std::pair<int, T>(index, item));
}

template <typename T> template <typename Y>
T const & ContainerVector<T>::remove(int index)
{
	for(auto it = items.begin(); it != items.end(); ++it)
	{
		if(*it == item)
		{
			removes.insert(it);
			return *it;
		}
	}
	throw std::exception();
}

template <typename T>
void ContainerVector<T>::clear()
{
	for(auto it = items.begin(); it != items.end(); ++it)
	{
		removes.push_back(it);
	}
}

template <typename T>
void ContainerVector<T>::processRemoves()
{
	for(auto it : removes)
	{
		l.erase(it);
	}
	removes.clear();
}

template <typename T>
typename std::list<T>::iterator ContainerVector<T>::begin()
{
	return items.begin();
}

template <typename T>
typename std::list<T>::iterator ContainerVector<T>::end()
{
	return items.end();
}

template <typename T>
typename std::list<T>::const_iterator ContainerVector<T>::begin() const
{
	return items.cbegin();
}

template <typename T>
typename std::list<T>::const_iterator ContainerVector<T>::end() const
{
	return items.cend();
}

template <typename T>
typename std::list<T>::reverse_iterator ContainerVector<T>::rbegin()
{
	return items.rbegin();
}

template <typename T>
typename std::list<T>::reverse_iterator ContainerVector<T>::rend()
{
	return items.rend();
}

template <typename T>
typename std::list<T>::const_reverse_iterator ContainerVector<T>::rbegin() const
{
	return items.crbegin();
}

template <typename T>
typename std::list<T>::const_reverse_iterator ContainerVector<T>::rend() const
{
	return items.crend();
}

template <typename T>
class ContainerVector<T>::IteratorHash
{
public:
	size_t operator () (typename std::list<T>::const_iterator const & it)
	{
		return (unsigned int)(*it);
	}
};

