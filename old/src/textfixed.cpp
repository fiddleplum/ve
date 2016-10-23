#include "text.h"
#include <fstream>
#include <locale>

namespace ve
{
	Text::Text()
	{
	}

	Text::Text(Text const & text)
	{
		s = text.s;
	}

	Text::Text(uint32_t codePoint)
	{
		s.push_back(codePoint);
	}

	Text::Text(char const * cs)
	{
		s.insert(s.end(), cs, cs + strlen(cs));
	}

	Text::Text(std::string const & string)
	{
		size_t i = 0;
		while (i < string.size())
		{
			int numBytes = 0;
			uint32_t codePoint = 0;
			if (string[i] <= 0x7f) // 0 to 7f
			{
				numBytes = 1;
				codePoint = string[i] & 0x7f;
			}
			else if ((string[i] & 0xe0) == 0xc0 && i + 1 < string.size()) // 80 to 7ff
			{
				numBytes = 2;
				codePoint = ((string[i] & 0x1f) << 6) + (string[i + 1] & 0x3f);
			}
			else if ((string[i] & 0xf0) == 0xe0 && i + 2 < string.size()) // 800 to ffff
			{
				numBytes = 3;
				codePoint = ((string[i] & 0x0f) << 12) + ((string[i + 1] & 0x3f) << 6) + (string[i + 2] & 0x3f);
			}
			else if ((string[i] & 0xf8) == 0xf0 && i + 3 < string.size()) // 10000 to 1fffff
			{
				numBytes = 4;
				codePoint = ((string[i] & 0x07) << 18) + ((string[i + 1] & 0x3f) << 12) + ((string[i + 2] & 0x3f) << 6) + (string[i + 3] & 0x3f);
			}
			else if ((string[i] & 0xfc) == 0xf8 && i + 4 < string.size()) // 200000 to 3ffffff
			{
				numBytes = 5;
				codePoint = ((string[i] & 0x03) << 24) + ((string[i + 1] & 0x3f) << 18) + ((string[i + 2] & 0x3f) << 12) + ((string[i + 3] & 0x3f) << 6) + (string[i + 4] & 0x3f);
			}
			else if ((string[i] & 0xfe) == 0xfc && i + 5 < string.size()) // 4000000 to 7fffffff
			{
				numBytes = 6;
				codePoint = ((string[i] & 0x01) << 30) + ((string[i + 1] & 0x3f) << 24) + ((string[i + 2] & 0x3f) << 18) + ((string[i + 3] & 0x3f) << 12) + ((string[i + 4] & 0x3f) << 6) + (string[i + 5] & 0x3f);
			}
			else
			{
				break;
			}
			s.push_back(codePoint);
			i += numBytes;
		}
	}

	Text & Text::operator = (Text const & text)
	{
		s = text.s;
		return *this;
	}

	unsigned int Text::size() const
	{
		return (unsigned int)s.size();
	}

	void Text::operator += (Text const & text)
	{
		s.insert(s.end(), text.s.begin(), text.s.end());
	}

	std::string Text::toString() const
	{
		std::string result;
		for (size_t i = 0; i < s.size(); i++)
		{
			uint32_t codePoint = s[i];
			if (codePoint <= 0x7f)
			{
				result.push_back(codePoint & 0x7f);
			}
			else if (codePoint <= 0x7ff)
			{
				result.push_back(0xc0 | ((codePoint >> 6) & 0x1f));
				result.push_back(0x80 | (codePoint & 0x3f));
			}
			else if (codePoint <= 0xffff)
			{
				result.push_back(0xe0 | ((codePoint >> 12) & 0x0f));
				result.push_back(0x80 | ((codePoint >> 6) & 0x3f));
				result.push_back(0x80 | (codePoint & 0x3f));
			}
			else if (codePoint <= 0x1fffff)
			{
				result.push_back(0xf0 | ((codePoint >> 18) & 0x07));
				result.push_back(0x80 | ((codePoint >> 12) & 0x3f));
				result.push_back(0x80 | ((codePoint >> 6) & 0x3f));
				result.push_back(0x80 | (codePoint & 0x3f));
			}
			else if (codePoint <= 0x3ffffff)
			{
				result.push_back(0xf8 | ((codePoint >> 24) & 0x03));
				result.push_back(0x80 | ((codePoint >> 18) & 0x3f));
				result.push_back(0x80 | ((codePoint >> 12) & 0x3f));
				result.push_back(0x80 | ((codePoint >> 6) & 0x3f));
				result.push_back(0x80 | (codePoint & 0x3f));
			}
			else if (codePoint <= 0x7fffffff)
			{
				result.push_back(0xfc | ((codePoint >> 30) & 0x01));
				result.push_back(0x80 | ((codePoint >> 24) & 0x3f));
				result.push_back(0x80 | ((codePoint >> 18) & 0x3f));
				result.push_back(0x80 | ((codePoint >> 12) & 0x3f));
				result.push_back(0x80 | ((codePoint >> 6) & 0x3f));
				result.push_back(0x80 | (codePoint & 0x3f));
			}
		}
		return result;
	}

	void Text::readFile(Text const & filename)
	{
		std::locale loc;
		std::fstream in(loc.widen(filename.toString()0, std::fstream::in || std::fstream::binary);
	}

	int Text::compare(Text const & text) const
	{
		if (s.size() < text.s.size())
		{
			if (memcmp(&s[0], &text.s[0], s.size()) < 0)
			{
				return -1;
			}
			return +1;
		}
		if (s.size() > text.s.size())
		{
			if (memcmp(&s[0], &text.s[0], text.s.size()) > 0)
			{
				return +1;
			}
			return -1;
		}
		return memcmp(&s[0], &text.s[0], s.size());
	}

	bool Text::beginsWith(Text const & text) const
	{
		if (s.size() < text.s.size())
		{
			return false;
		}
		return memcmp(&s[0], &text.s[0], text.s.size()) == 0;
	}

	bool Text::endsWith(Text const & text) const
	{
		if (s.size() < text.s.size())
		{
			return false;
		}
		return memcmp(&s[s.size() - text.s.size()], &text.s[0], text.s.size()) == 0;
	}

	void Text::clear()
	{
		s.clear();
	}

	void Text::trim()
	{
		size_t i;
		for (i = 0; i < s.size(); i++)
		{
			uint32_t c = s[i];
			if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
			{
				break;
			}
		}
		s.erase(s.begin(), s.begin() + i);
		for (i = s.size() - 1; i > 0; i--)
		{
			uint32_t c = s[i];
			if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
			{
				break;
			}
		}
		s.erase(s.begin() + i, s.end());
	}

	std::vector<uint32_t>::iterator Text::begin()
	{
		return s.begin();
	}

	std::vector<uint32_t>::const_iterator Text::begin() const
	{
		return s.begin();
	}

	std::vector<uint32_t>::iterator Text::end()
	{
		return s.end();
	}

	std::vector<uint32_t>::const_iterator Text::end() const
	{
		return s.end();
	}

	bool operator == (Text const & lhs, Text const & rhs)
	{
		return lhs.compare(rhs) == 0;
	}

	bool operator != (Text const & lhs, Text const & rhs)
	{
		return lhs.compare(rhs) != 0;
	}

	bool operator < (Text const & lhs, Text const & rhs)
	{
		return lhs.compare(rhs) < 0;
	}

	bool operator <= (Text const & lhs, Text const & rhs)
	{
		return lhs.compare(rhs) <= 0;
	}

	bool operator > (Text const & lhs, Text const & rhs)
	{
		return lhs.compare(rhs) > 0;
	}

	bool operator >= (Text const & lhs, Text const & rhs)
	{
		return lhs.compare(rhs) >= 0;
	}

	Text operator + (Text const & lhs, Text const & rhs)
	{
		Text result {lhs};
		result += rhs;
		return result;
	}
}

