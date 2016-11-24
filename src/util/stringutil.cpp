#include "util/stringutil.hpp"
#include <sstream>
#include <fstream>

namespace ve
{
	// Many of these algorithms are the same as for ASCII, because any UTF-8 character cannot be within any other character.

	std::string readFile(std::string const & filename)
	{
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in.fail())
		{
			throw std::runtime_error("Could not open file '" + filename + "'. ");
		}
		std::stringstream ss;
		ss << in.rdbuf();
		return ss.str();
	}

	void writeFile(std::string const & filename, std::string const & content)
	{
		std::ofstream out(filename, std::ios::out | std::ios::binary);
		out << content;
	}

	bool beginsWith(std::string const & subject, std::string const & part)
	{
		return subject.compare(0, part.length(), part) == 0;
	}

	bool endsWith(std::string const & subject, std::string const & part)
	{
		if (part.length() < subject.length())
		{
			return subject.compare(subject.length() - part.length(), part.length(), part) == 0;
		}
		return false;
	}

	std::vector<std::string> split(std::string const & subject, std::string const & delimiter)
	{
		size_t i = 0;
		std::vector<std::string> result;
		do
		{
			auto iNext = subject.find(delimiter, i);
			result.push_back(subject.substr(i, iNext - i));
			if (iNext == subject.npos)
			{
				break;
			}
			i = iNext + delimiter.length();
		} while (true);
		return result;
	}

	std::string trim(std::string const & subject, bool left, bool right)
	{
		std::string result = subject;
		std::string delimiters = " \t\r\n";
		if (left)
		{
			result.erase(0, result.find_first_not_of(delimiters));
		}
		if (right)
		{
			result.erase(result.find_last_not_of(delimiters) + 1);
		}
		return result;
	}

	unsigned int getChar(std::string const & subject, size_t & i)
	{
		uint32_t codePoint = 0;
		if (i < subject.length())
		{
			if (subject[i] <= 0x7f) // 0 to 7f
			{
				codePoint = subject[i] & 0x7f;
				i += 1;
			}
			else if ((subject[i] & 0xe0) == 0xc0 && i + 1 < subject.length()) // 80 to 7ff
			{
				codePoint = ((unsigned int)(subject[i] & 0x1f) << 6) + (unsigned int)(subject[i + 1] & 0x3f);
				i += 2;
			}
			else if ((subject[i] & 0xf0) == 0xe0 && i + 2 < subject.length()) // 800 to ffff
			{
				codePoint = ((unsigned int)(subject[i] & 0x0f) << 12) + ((unsigned int)(subject[i + 1] & 0x3f) << 6) + (unsigned int)(subject[i + 2] & 0x3f);
				i += 3;
			}
			else if ((subject[i] & 0xf8) == 0xf0 && i + 3 < subject.length()) // 10000 to 1fffff
			{
				codePoint = ((unsigned int)(subject[i] & 0x07) << 18) + ((unsigned int)(subject[i + 1] & 0x3f) << 12) + ((unsigned int)(subject[i + 2] & 0x3f) << 6) + (unsigned int)(subject[i + 3] & 0x3f);
				i += 4;
			}
			else if ((subject[i] & 0xfc) == 0xf8 && i + 4 < subject.length()) // 200000 to 3ffffff
			{
				codePoint = ((unsigned int)(subject[i] & 0x03) << 24) + ((unsigned int)(subject[i + 1] & 0x3f) << 18) + ((unsigned int)(subject[i + 2] & 0x3f) << 12) + ((unsigned int)(subject[i + 3] & 0x3f) << 6) + (unsigned int)(subject[i + 4] & 0x3f);
				i += 5;
			}
			else if ((subject[i] & 0xfe) == 0xfc && i + 5 < subject.length()) // 4000000 to 7fffffff
			{
				codePoint = ((unsigned int)(subject[i] & 0x01) << 30) + ((unsigned int)(subject[i + 1] & 0x3f) << 24) + ((unsigned int)(subject[i + 2] & 0x3f) << 18) + ((unsigned int)(subject[i + 3] & 0x3f) << 12) + ((unsigned int)(subject[i + 4] & 0x3f) << 6) + (unsigned int)(subject[i + 5] & 0x3f);
				i += 6;
			}
		}
		return codePoint;
	}

	std::string getStringFromChar(unsigned int c)
	{
		std::string s;
		if (c <= 0x7f)
		{
			s.push_back(c & 0x7f);
		}
		else if (c <= 0x7ff)
		{
			s.push_back(0xc0 | ((c >> 6) & 0x1f));
			s.push_back(0x80 | (c & 0x3f));
		}
		else if (c <= 0xffff)
		{
			s.push_back(0xe0 | ((c >> 12) & 0x0f));
			s.push_back(0x80 | ((c >> 6) & 0x3f));
			s.push_back(0x80 | (c & 0x3f));
		}
		else if (c <= 0x1fffff)
		{
			s.push_back(0xf0 | ((c >> 18) & 0x07));
			s.push_back(0x80 | ((c >> 12) & 0x3f));
			s.push_back(0x80 | ((c >> 6) & 0x3f));
			s.push_back(0x80 | (c & 0x3f));
		}
		else if (c <= 0x3ffffff)
		{
			s.push_back(0xf8 | ((c >> 24) & 0x03));
			s.push_back(0x80 | ((c >> 18) & 0x3f));
			s.push_back(0x80 | ((c >> 12) & 0x3f));
			s.push_back(0x80 | ((c >> 6) & 0x3f));
			s.push_back(0x80 | (c & 0x3f));
		}
		else if (c <= 0x7fffffff)
		{
			s.push_back(0xfc | ((c >> 30) & 0x01));
			s.push_back(0x80 | ((c >> 24) & 0x3f));
			s.push_back(0x80 | ((c >> 18) & 0x3f));
			s.push_back(0x80 | ((c >> 12) & 0x3f));
			s.push_back(0x80 | ((c >> 6) & 0x3f));
			s.push_back(0x80 | (c & 0x3f));
		}
		return s;
	}

	size_t getCharSize(std::string const & subject, size_t i)
	{
		size_t byteSize = 0;
		if (i != subject.size())
		{
			unsigned int firstCodePointByte = subject[i];
			if (firstCodePointByte <= 0x7f) // 0 to 7f
			{
				byteSize = 1;
			}
			else if ((firstCodePointByte & 0xe0) == 0xc0) // 80 to 7ff
			{
				byteSize = 2;
			}
			else if ((firstCodePointByte & 0xf0) == 0xe0) // 800 to ffff
			{
				byteSize = 3;
			}
			else if ((firstCodePointByte & 0xf8) == 0xf0) // 10000 to 1fffff
			{
				byteSize = 4;
			}
			else if ((firstCodePointByte & 0xfc) == 0xf8) // 200000 to 3ffffff
			{
				byteSize = 5;
			}
			else if ((firstCodePointByte & 0xfe) == 0xfc) // 4000000 to 7fffffff
			{
				byteSize = 6;
			}
		}
		return byteSize;
	}

	size_t getPrevCharSize(std::string const & subject, size_t i)
	{
		while (i != 0 && (subject[i] & 0xc0) == 0x80)
		{
			i--;
		}
		return getCharSize(subject, i);
	}

	std::string readUntil(std::string const & content, size_t & i, std::string const & delimiter)
	{
		size_t iNext = content.find(delimiter, i);
		if (iNext == std::string::npos)
		{
			iNext = content.size();
		}
		std::string ret = content.substr(i, iNext);
		i = iNext;
		return ret;
	}

	std::string readUntilAny(std::string const & content, size_t & i, std::string const & delimiters)
	{
		size_t iNext = content.find_first_of(delimiters, i);
		if (iNext == std::string::npos)
		{
			iNext = content.size();
		}
		std::string ret = content.substr(i, iNext);
		i = iNext;
		return ret;
	}

	void skipWhiteSpace(std::string const & content, size_t & i)
	{
		i = content.find_first_not_of(" \t\r\n", i);
		if (i == std::string::npos)
		{
			i = content.size();
		}
	}

	bool isStringNext(std::string const & content, size_t i, std::string const & token)
	{
		return content.compare(i, token.length(), token) == 0;
	}
}