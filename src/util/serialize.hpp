#pragma once

#include <istream>
#include <ostream>
#include <exception>
#include <string>
#include <vector>

namespace ve
{
	// Serialize a block of bytes.
	inline void serialize(std::ostream & out, void const * bytes, int numBytes)
	{
		out.write((char const *)bytes, numBytes);
		if (!out.good())
		{
			throw std::exception();
		}
	}

	// Deserialize a block of bytes.
	inline void deserialize(std::istream & in, void * bytes, int numBytes)
	{
		in.read((char *)bytes, numBytes);
		if (!in.good())
		{
			throw std::exception();
		}
	}

	// Serialize a bool.
	inline void serialize(std::ostream & out, bool const & v)
	{
		serialize(out, (void const *)&v, 1);
	}

	// Serialize an int.
	inline void serialize(std::ostream & out, int const & v)
	{
		serialize(out, (void const *)&v, 4);
	}

	// Serialize an unsigned int.
	inline void serialize(std::ostream & out, unsigned int const & v)
	{
		serialize(out, (void const *)&v, 4);
	}

	// Serialize a float.
	inline void serialize(std::ostream & out, float const & v)
	{
		serialize(out, (void const *)&v, 4);
	}

	// Serialize a double.
	inline void serialize(std::ostream & out, double const & v)
	{
		serialize(out, (void const *)&v, 8);
	}

	// Deserialize a bool.
	inline void deserialize(std::istream & in, bool & v)
	{
		v = 0;
		deserialize(in, (void *)&v, 1);
	}

	// Deserialize an int.
	inline void deserialize(std::istream & in, int & v)
	{
		v = 0;
		deserialize(in, (void *)&v, 4);
	}

	// Deserialize an unsigned int.
	inline void deserialize(std::istream & in, unsigned int & v)
	{
		v = 0;
		deserialize(in, (void *)&v, 4);
	}

	// Deserialize a float.
	inline void deserialize(std::istream & in, float & v)
	{
		v = 0;
		deserialize(in, (void *)&v, 4);
	}

	// Deserialize a double.
	inline void deserialize(std::istream & in, double & v)
	{
		v = 0;
		deserialize(in, (void *)&v, 8);
	}

	// Serialize a string.
	inline void serialize(std::ostream & out, std::string const & v)
	{
		serialize(out, (int)v.size());
		serialize(out, (void const *)&v[0], (int)v.size());
	}

	// Deserialize a string.
	inline void deserialize(std::istream & in, std::string & v)
	{
		int size;
		deserialize(in, size);
		v.resize((size_t)size);
		deserialize(in, (void *)&v[0], size);
	}

	// Serialize a vector of items using a serialize function for each item.
	template <class T>
	void serialize(std::ostream & out, std::vector<T> const & v, void(*serializeItem)(std::ostream &, T const &) = serialize)
	{
		serialize(out, (int)v.size());
		for (T const & item : v)
		{
			serializeItem(out, item);
		}
	}

	// Deserialize a vector of items using a deserialize function for each item.
	template <class T>
	void deserialize(std::istream & in, std::vector<T> & v, void(*deserializeItem)(std::istream &, T &) = deserialize)
	{
		int size;
		deserialize(in, size);
		v.resize((size_t)size);
		for (T & item : v)
		{
			deserializeItem(in, item);
		}
	}
}