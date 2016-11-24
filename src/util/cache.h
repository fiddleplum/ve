#pragma once

#include "util/ptr.h"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace ve
{
	// Generic object cache.
	template <typename T>
	class Cache
	{
	public:
		// Constructor. O(1)
		Cache();

		// Destructor. Calls clean(). Throws an exception if it still has any objects. O(number of objects)
		~Cache();

		// Returns the named object or null if it is not found. O(log number of objects)
		Ptr<T> get(std::string const & name) const;

		// Constructs and returns the named object. If it already exists, an exception is thrown. O(log number of objects) + O(constructor)
		template <typename ... Args> Ptr<T> create(std::string const & name, Args && ... args);

		// Removes and destroys the objects that aren't referenced outside of the cache. O(number of objects).
		void clean();

		// Returns a list of objects in the cache by name.
		std::vector<std::string> list() const;

	private:
		std::map<std::string, OwnPtr<T>> objects;
	};

	// Template Implementations

	template <typename T>
	Cache<T>::Cache()
	{
	}

	template <typename T>
	Cache<T>::~Cache()
	{
		clean();
		if (!objects.empty())
		{
			//std::string message = "There are still objects referenced:\n";
			//for (auto const & pair : objects)
			//{
			//	message += pair.first + "\n";
			//}
			//throw std::runtime_error(message);
		}
	}

	template <typename T>
	Ptr<T> Cache<T>::get(std::string const & name) const
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			return it->second;
		}
		else
		{
			return Ptr<T>();
		}
	}

	template <typename T> template <typename ... Args>
	Ptr<T> Cache<T>::create(std::string const & name, Args && ... args)
	{
		auto it = objects.find(name);
		if (it == objects.end())
		{
			OwnPtr<T> object;
			try
			{
				object.setNew(std::forward<Args>(args)...);
			}
			catch (std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			objects[name] = object;
			return object;
		}
		else
		{
			throw std::runtime_error("'" + name + "' is already in the cache.");
		}
	}

	template <typename T>
	void Cache<T>::clean()
	{
		for (auto it = objects.begin(); it != objects.end();)
		{
			if (!it->second.isInUse())
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
	std::vector<std::string> Cache<T>::list() const
	{
		std::vector<std::string> names;
		for (auto const & pair : objects)
		{
			names.push_back(pair.first);
		}
		return names;
	}
}