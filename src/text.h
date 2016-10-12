#pragma once

#include <vector>

namespace ve
{
	// A UTF-8 encoded string of text. std::string is not used because its iterators do not work with multi-byte code points.
	class Text
	{
	public:
		class const_iterator;

		class iterator;

		Text();

		Text(Text const & text);

		Text(std::string const & string);

		Text(char const * cs);

		Text(uint32_t codePoint);

		Text(char c);

		Text & operator = (Text const & text);

		void operator += (Text const & text);

		unsigned int byteSize() const;

		unsigned int size() const;

		bool empty() const;

		void clear();

		int compare(Text const & text) const;

		void erase(iterator start, iterator end);

		void insert(iterator where, Text const & text);

		const_iterator find(Text const & text, const_iterator const & start) const;

		iterator find(Text const & text, iterator const & start);

		Text substring(const_iterator start, const_iterator end) const;

		//std::wstring toWideString() const;

		const_iterator begin() const;

		iterator begin();

		const_iterator end() const;

		iterator end();

		const_iterator cbegin() const;

		const_iterator cend() const;

		class const_iterator
		{
		public:
			const_iterator(Text const & text, std::vector<uint8_t>::const_iterator const & iter);

			const_iterator(const_iterator const & iter);

			const_iterator(iterator const & iter);

			void operator ++ (int);

			void operator -- (int);

			uint32_t operator * () const;

			bool operator != (const_iterator const & iter) const;

		private:
			Text const & text;
			std::vector<uint8_t>::const_iterator iter;
			friend Text;
		};

		class iterator
		{
		public:
			iterator(Text & text, std::vector<uint8_t>::iterator const & iter);

			iterator(iterator const & iter);

			void operator ++ (int);

			void operator -- (int);

			uint32_t operator * () const;

			bool operator != (iterator const & iter) const;

		private:
			Text & text;
			std::vector<uint8_t>::iterator iter;
			friend Text;
		};

	private:
		std::vector<uint8_t> v;
		unsigned int numCodePoints = 0;
	};

	Text operator + (Text const & lhs, Text const & rhs);

	bool operator == (Text const & lhs, Text const & rhs);

	bool operator != (Text const & lhs, Text const & rhs);

	bool operator < (Text const & lhs, Text const & rhs);

	bool operator <= (Text const & lhs, Text const & rhs);

	bool operator > (Text const & lhs, Text const & rhs);

	bool operator >= (Text const & lhs, Text const & rhs);
}

