#include <string>
#include <vector>

namespace ve
{
	// Returns true if the subject ends with the ending.
	bool beginsWith(std::string const & subject, std::string const & ending);

	// Returns true if the subject ends with the ending.
	bool endsWith(std::string const & subject, std::string const & ending);

	// Split a string by a delimiter and put it in result.
	void split(std::string const & subject, char delimiter, std::vector<std::string> & result);

	// Split a string by a delimiter and return the result.
	std::vector<std::string> split(std::string const & subject, char delimiter);

	// Load the text from a file and put it in result.
	void readFile(std::string const & filename, std::string & result);

	// Load the text from a file and return the result.
	std::string readFile(std::string const & filename);
}

