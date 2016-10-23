#include "text.h"
#include <algorithm>

namespace ve
{
	// Returns the byte size of the character starting at i.
	unsigned int getNextByteSize(std::vector<uint8_t> const & v, std::vector<uint8_t>::const_iterator const & i)
	{
		unsigned int byteSize = 0;
		if (i != v.end())
		{
			unsigned int firstCodePointByte = *i;
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

	// Returns the byte size of the character preceding i.
	unsigned int getPrevByteSize(std::vector<uint8_t> const & v, std::vector<uint8_t>::const_iterator const & i)
	{
		auto pi = i;
		while (pi != v.begin() && (*pi & 0xc0) == 0x80)
		{
			pi--;
		}
		return getNextByteSize(v, pi);
	}

	uint32_t getCodePoint(std::vector<uint8_t> const & v, std::vector<uint8_t>::const_iterator const & i)
	{
		uint32_t codePoint = 0;
		if (i != v.end())
		{
			if (*i <= 0x7f) // 0 to 7f
			{
				codePoint = *i & 0x7f;
			}
			else if ((*i & 0xe0) == 0xc0 && std::distance(v.end(), i) >= 2) // 80 to 7ff
			{
				codePoint = ((uint32_t)(*i & 0x1f) << 6) + (uint32_t)(*(i + 1) & 0x3f);
			}
			else if ((*i & 0xf0) == 0xe0 && std::distance(v.end(), i) >= 3) // 800 to ffff
			{
				codePoint = ((uint32_t)(*i & 0x0f) << 12) + ((uint32_t)(*(i + 1) & 0x3f) << 6) + (uint32_t)(*(i + 2) & 0x3f);
			}
			else if ((*i & 0xf8) == 0xf0 && std::distance(v.end(), i) >= 4) // 10000 to 1fffff
			{
				codePoint = ((uint32_t)(*i & 0x07) << 18) + ((uint32_t)(*(i + 1) & 0x3f) << 12) + ((uint32_t)(*(i + 2) & 0x3f) << 6) + (uint32_t)(*(i + 3) & 0x3f);
			}
			else if ((*i & 0xfc) == 0xf8 && std::distance(v.end(), i) >= 5) // 200000 to 3ffffff
			{
				codePoint = ((uint32_t)(*i & 0x03) << 24) + ((uint32_t)(*(i + 1) & 0x3f) << 18) + ((uint32_t)(*(i + 2) & 0x3f) << 12) + ((uint32_t)(*(i + 3) & 0x3f) << 6) + (uint32_t)(*(i + 4) & 0x3f);
			}
			else if ((*i & 0xfe) == 0xfc && std::distance(v.end(), i) >= 6) // 4000000 to 7fffffff
			{
				codePoint = ((uint32_t)(*i & 0x01) << 30) + ((uint32_t)(*(i + 1) & 0x3f) << 24) + ((uint32_t)(*(i + 2) & 0x3f) << 18) + ((uint32_t)(*(i + 3) & 0x3f) << 12) + ((uint32_t)(*(i + 4) & 0x3f) << 6) + (uint32_t)(*(i + 5) & 0x3f);
			}
		}
		return codePoint;
	}

	Text::Text()
	{
	}

	Text::Text(Text const & text)
		: v {text.v}, numCodePoints {text.numCodePoints}
	{
	}

	Text::Text(std::string const & string)
		: v {string.begin(), string.end()}
	{
		const_iterator iter = begin();
		while (iter != end())
		{
			numCodePoints++;
			iter++;
		}
	}

	Text::Text(char const * cs)
		: v {cs, cs + strlen(cs)}
	{
		const_iterator iter = begin();
		while (iter != end())
		{
			numCodePoints++;
			iter++;
		}
	}

	Text::Text(uint32_t codePoint)
	{
		if (codePoint <= 0x7f)
		{
			v.push_back(codePoint & 0x7f);
		}
		else if (codePoint <= 0x7ff)
		{
			v.push_back(0xc0 | ((codePoint >> 6) & 0x1f));
			v.push_back(0x80 | (codePoint & 0x3f));
		}
		else if (codePoint <= 0xffff)
		{
			v.push_back(0xe0 | ((codePoint >> 12) & 0x0f));
			v.push_back(0x80 | ((codePoint >> 6) & 0x3f));
			v.push_back(0x80 | (codePoint & 0x3f));
		}
		else if (codePoint <= 0x1fffff)
		{
			v.push_back(0xf0 | ((codePoint >> 18) & 0x07));
			v.push_back(0x80 | ((codePoint >> 12) & 0x3f));
			v.push_back(0x80 | ((codePoint >> 6) & 0x3f));
			v.push_back(0x80 | (codePoint & 0x3f));
		}
		else if (codePoint <= 0x3ffffff)
		{
			v.push_back(0xf8 | ((codePoint >> 24) & 0x03));
			v.push_back(0x80 | ((codePoint >> 18) & 0x3f));
			v.push_back(0x80 | ((codePoint >> 12) & 0x3f));
			v.push_back(0x80 | ((codePoint >> 6) & 0x3f));
			v.push_back(0x80 | (codePoint & 0x3f));
		}
		else if (codePoint <= 0x7fffffff)
		{
			v.push_back(0xfc | ((codePoint >> 30) & 0x01));
			v.push_back(0x80 | ((codePoint >> 24) & 0x3f));
			v.push_back(0x80 | ((codePoint >> 18) & 0x3f));
			v.push_back(0x80 | ((codePoint >> 12) & 0x3f));
			v.push_back(0x80 | ((codePoint >> 6) & 0x3f));
			v.push_back(0x80 | (codePoint & 0x3f));
		}
		numCodePoints = 1;
	}

	Text::Text(char c)
	{
		v.push_back(c);
		numCodePoints = 1;
	}

	Text & Text::operator = (Text const & text)
	{
		v = text.v;
		numCodePoints = text.numCodePoints;
		return *this;
	}

	void Text::operator += (Text const & text)
	{
		v.insert(v.end(), text.v.begin(), text.v.end());
		numCodePoints += text.numCodePoints;
	}

	unsigned int Text::byteSize() const
	{
		return (unsigned int)v.size();
	}

	unsigned int Text::size() const
	{
		return numCodePoints;
	}

	bool Text::empty() const
	{
		return v.empty();
	}

	void Text::clear()
	{
		v.clear();
		numCodePoints = 0;
	}

	int Text::compare(Text const & text) const
	{
		if (v.size() < text.v.size())
		{
			if (memcmp(&v[0], &text.v[0], v.size()) > 0)
			{
				return +1;
			}
			return -1;
		}
		if (v.size() > text.v.size())
		{
			if (memcmp(&v[0], &text.v[0], text.v.size()) < 0)
			{
				return -1;
			}
			return +1;
		}
		return memcmp(&v[0], &text.v[0], v.size());
	}

	void Text::erase(iterator start, iterator end)
	{
		auto iter = start;
		while (iter != end)
		{
			numCodePoints--;
			iter++;
		}
		v.erase(start.iter, end.iter);
	}

	void Text::insert(iterator where, Text const & text)
	{
		v.insert(where.iter, text.v.begin(), text.v.end());
		numCodePoints += text.numCodePoints;
	}

	Text::const_iterator Text::find(Text const & text, const_iterator const & start) const
	{
		return {*this, std::search(start.iter, v.end(), text.v.begin(), text.v.end())};
	}

	Text::iterator Text::find(Text const & text, iterator const & start)
	{
		return {*this, std::search(start.iter, v.end(), text.v.begin(), text.v.end())};
	}

	Text Text::substring(const_iterator start, const_iterator end) const
	{
		Text text;
		text.v.insert(text.v.begin(), start.iter, end.iter);
		return text;
	}

	//std::wstring Text::toWideString() const;

	Text::const_iterator Text::begin() const
	{
		return {*this, v.begin()};
	}

	Text::iterator Text::begin()
	{
		return {*this, v.begin()};
	}

	Text::const_iterator Text::end() const
	{
		return {*this, v.end()};
	}

	Text::iterator Text::end()
	{
		return {*this, v.end()};
	}

	Text::const_iterator Text::cbegin() const
	{
		return {*this, v.cbegin()};
	}

	Text::const_iterator Text::cend() const
	{
		return {*this, v.cend()};
	}

	Text::const_iterator::const_iterator(Text const & text_, std::vector<uint8_t>::const_iterator const & iter_)
		: text(text_), iter {iter_}
	{
	}

	Text::const_iterator::const_iterator(const_iterator const & iter_)
		: text {iter_.text}, iter {iter_.iter}
	{
	}

	Text::const_iterator::const_iterator(iterator const & iter_)
		: text {iter_.text}, iter {iter_.iter}
	{
	}

	void Text::const_iterator::operator ++ (int)
	{
		iter += getNextByteSize(text.v, iter);;
	}

	void Text::const_iterator::operator -- (int)
	{
		iter -= getPrevByteSize(text.v, iter);
	}

	uint32_t Text::const_iterator::operator * () const
	{
		return getCodePoint(text.v, iter);
	}

	bool Text::const_iterator::operator != (const_iterator const & iter_) const
	{
		return &text != &iter_.text || iter != iter_.iter;
	}

	Text::iterator::iterator(Text & text_, std::vector<uint8_t>::iterator const & iter_)
		: text {text_}, iter {iter_}
	{
	}

	Text::iterator::iterator(iterator const & iter_)
		: text {iter_.text}, iter {iter_.iter}
	{
	}

	void Text::iterator::operator ++ (int)
	{
		iter += getNextByteSize(text.v, iter);
	}

	void Text::iterator::operator -- (int)
	{
		iter -= getPrevByteSize(text.v, iter);
	}

	uint32_t Text::iterator::operator * () const
	{
		return getCodePoint(text.v, iter);
	}

	bool Text::iterator::operator != (iterator const & iter_) const
	{
		return &text != &iter_.text || iter != iter_.iter;
	}
}

