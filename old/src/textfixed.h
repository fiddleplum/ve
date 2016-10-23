#pragma once

#include <vector>
#include <string>

namespace ve
{
	// A Unicode string of text. Uses code points and is not encoded.
	class Text
	{
	public:
		// Default constructor.
		Text();

		// Copy constructor.
		Text(Text const & text);

		// Constructs from a code point.
		Text(uint32_t codePoint);

		// Constructs from an ANSII string.
		Text(char const * cs);

		// Constructs from a UTF-8 string.
		Text(std::string const & utf8);

		// Copy operator.
		Text & operator = (Text const & text);

		// Returns the number of characters.
		unsigned int size() const;

		// Adds text.
		void operator += (Text const & text);

		// Converts to a utf8 string.
		std::string toString() const;

		// Sets this to the contents of a utf8 encoded file.
		void readFile(Text const & filename);

		// Compares this string. Returns -1 for less, 0 for equal, and +1 for greater.
		int compare(Text const & text) const;

		// Returns true if it begins with the text.
		bool beginsWith(Text const & text) const;

		// Returns true if it ends with the text.
		bool endsWith(Text const & text) const;

		// Clears the text.
		void clear();

		// Removes white space from the text.
		void trim();

		// Returns an iterator to the first character of the text, or end.
		std::vector<uint32_t>::iterator begin();

		// Returns a const iterator to the first character of the text, or end.
		std::vector<uint32_t>::const_iterator begin() const;

		// Returns an iterator to after the last character of the text.
		std::vector<uint32_t>::iterator end();

		// Returns a const iterator to after the last character of the text.
		std::vector<uint32_t>::const_iterator end() const;

	private:
		void putChar(size_t i, uint32_t codePoint);

		std::vector<uint32_t> s;
	};

	// Returns true if lhs is equal to rhs.
	bool operator == (Text const & lhs, Text const & rhs);

	// Returns true if lhs is not equal to rhs.
	bool operator != (Text const & lhs, Text const & rhs);

	// Returns true if lhs is less than rhs.
	bool operator < (Text const & lhs, Text const & rhs);

	// Returns true if lhs is less than or equal to rhs.
	bool operator <= (Text const & lhs, Text const & rhs);

	// Returns true if lhs is greater than rhs.
	bool operator > (Text const & lhs, Text const & rhs);

	// Returns true if lhs is greater than or equal to rhs.
	bool operator >= (Text const & lhs, Text const & rhs);

	// Adds a text and a text.
	Text operator + (Text const & lhs, Text const & rhs);
}

