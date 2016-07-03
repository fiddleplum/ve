#include "util.h"
#include <sstream>
#include <fstream>

namespace ve
{
	bool beginsWith(std::string const & subject, std::string const & ending)
	{
		return subject.compare(0, ending.length(), ending) == 0;
	}

	bool endsWith(std::string const & subject, std::string const & ending)
	{
		return (ending.size() <= subject.size()) && std::equal(ending.rbegin(), ending.rend(), subject.rbegin());
	}

	void split(std::string const & subject, char delimiter, std::vector<std::string> & result)
	{
		std::stringstream ss(subject);
		std::string item;
		while (std::getline(ss, item, delimiter))
		{
			result.push_back(item);
		}
	}

	std::vector<std::string> split(const std::string & subject, char delimiter)
	{
		std::vector<std::string> result;
		split(subject, delimiter, result);
		return result;
	}

	void readFile(std::string const & filename, std::string & result)
	{
		std::fstream in(filename, std::fstream::in);
		std::stringstream ss;
		ss << in.rdbuf();
		result = ss.str();
	}

	std::string readFile(std::string const & filename)
	{
		std::string result;
		readFile(filename, result);
		return result;
	}
}

