#pragma once

#include "util/ptr.h"
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

		// Returns the named object or null if it is not found. O(log number of objects)
		Ptr<Object> get(std::string const & name) const;

		// Constructs and returns the named object. If it already exists, an exception is thrown. O(log number of objects)
		template <typename ... Args> Ptr<Object> create(std::string const & name, Args const & ... args);

		// Constructs and returns the named object. If it already exists, an exception is thrown. O(log number of objects)
		template <typename ... Args> Ptr<Object> create(std::string const & name, Args & ... args);

		// Removes and destroys the objects that aren't referenced outside of the cache. O(number of objects).
		void clean();

		// Gets a list of objects in the cache by name.
		std::vector<std::string> getObjectNames() const;

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
			//std::string message = "There are still objects referenced:\n";
			//for (auto const & pair : objects)
			//{
			//	message += pair.first + "\n";
			//}
			//throw std::runtime_error(message);
		}
	}

	template <typename Object>
	Ptr<Object> Cache<Object>::get(std::string const & name) const
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			return it->second;
		}
		else
		{
			return Ptr<Object>();
		}
	}

	template <typename Object>
	template <typename ... Args>
	Ptr<Object> Cache<Object>::create(std::string const & name, Args const & ... args)
	{
		auto it = objects.find(name);
		if (it == objects.end())
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
		else
		{
			throw std::runtime_error("'" + name + "' is already in the cache.");
		}
	}

	template <typename Object>
	template <typename ... Args>
	Ptr<Object> Cache<Object>::create(std::string const & name, Args & ... args)
	{
		auto it = objects.find(name);
		if (it == objects.end())
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
		else
		{
			throw std::runtime_error("'" + name + "' is already in the cache.");
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
	std::vector<std::string> Cache<Object>::getObjectNames() const
	{
		std::vector<std::string> names;
		for (auto const & pair : objects)
		{
			names.push_back(pair.first);
		}
		return names;
	}
}