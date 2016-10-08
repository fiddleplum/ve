#pragma once

#include <string>
#include <vector>

// Throughout ve, std::string is encoded as UTF-8. Thus, special string functions need to be used for string manipulation.

namespace ve
{
	// Returns the content of a file.
	std::string readFile(std::string const & filename);

	// Writes the contents of a file.
	void writeFile(std::string const & filename, std::string const & content);

	// Returns true if the subject begins with the part.
	bool beginsWith(std::string const & subject, std::string const & part);

	// Returns true if the subject ends with the part.
	bool endsWith(std::string const & subject, std::string const & part);

	// Splits a string up by the delimiter string.
	std::vector<std::string> split(std::string const & subject, std::string const & delimiter);

	// Trims the string by whitespace (' ', '\t', '\r', '\n') for left or right sides.
	std::string trim(std::string const & subject, bool left = true, bool right = true);

	// Gets the character at the position.
	unsigned int getChar(std::string const & subject, size_t & i);

	// Gets the string specified by the character.
	std::string getStringFromChar(unsigned int c);

	// Returns the byte size of the character starting at i.
	size_t getCharSize(std::string const & subject, size_t i);

	// Returns the byte size of the character preceding i.
	size_t getPrevCharSize(std::string const & subject, size_t i);

	// Reads some text until the delimiter.
	std::string readUntil(std::string const & content, size_t & i, std::string const & delimiter);

	// Read some text until any char in the delimiter.
	std::string readUntilAny(std::string const & content, size_t & i, std::string const & delimiters);

	// Reads some text until white space (' ', '\t', '\r', '\n').
	void skipWhiteSpace(std::string const & content, size_t & i);

	// Returns true if the next characters are the token.
	bool isStringNext(std::string const & content, size_t i, std::string const & token);
}

