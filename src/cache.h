#pragma once

#include "ptr.h"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace ve
{
	// Generic resource cache.
	template <typename Object>
	class Cache
	{
	public:
		// Constructor. O(1)
		Cache();

		// Destructor. Calls clean(). Throws an exception if it still has any objects. O(number of objects)
		~Cache();

		// Returns true if an object with the given name is in the cache. O(log number of objects)
		bool has(std::string const & name) const;

		// Returns the named object. If it doesn't exist, an exception is thrown. O(log number of objects)
		UsePtr<Object> get(std::string const & name) const;

		// Returns the named object. If it doesn't exist, it creates the object using the args. O(log number of objects)
		template <typename... Args> UsePtr<Object> getOrCreate(std::string const & name, Args... args);

		// Removes and destroys the objects that aren't referenced outside of the cache. O(number of objects).
		void clean();

		// Gets a list of objects in the cache by name.
		std::vector<std::string> getObjectNames();

	private:
		std::map<std::string, OwnPtr<Object>> objects;
	};

	// Template Implementations

	template <typename Object>
	Cache<Object>::Cache()
	{
	}

	template <typename Object>
	Cache<Object>::~Cache()
	{
		clean();
		if (!objects.empty())
		{
			std::string message = "There are still objects referenced:\n";
			for (auto const & pair : objects)
			{
				message += pair.first + "\n";
			}
			throw std::runtime_error(message);
		}
	}

	template <typename Object>
	bool Cache<Object>::has(std::string const & name) const
	{
		return objects.find(name) != objects.end();
	}

	template <typename Object>
	UsePtr<Object> Cache<Object>::get(std::string const & name) const
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			return it->second;
		}
		else
		{
			throw std::runtime_error("'" + name + "' was not found in the cache.");
		}
	}

	template <typename Object>
	template <typename... Args>
	UsePtr<Object> Cache<Object>::getOrCreate(std::string const & name, Args... args)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Object> object;
			try
			{
				object.setNew(args...);
			}
			catch (std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			objects[name] = object;
			return object;
		}
	}

	template <typename Object>
	void Cache<Object>::clean()
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

	template <typename Object>
	std::vector<std::string> Cache<Object>::getObjectNames()
	{
		std::vector<std::string> names;
		for (auto const & pair : objects)
		{
			names.push_back(pair.first);
		}
		return names;
	}
}

