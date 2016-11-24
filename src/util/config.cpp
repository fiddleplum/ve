#include "util/config.hpp"
#include "util/stringutil.hpp"

namespace ve
{
	Config::Config()
	{
	}

	Config::Config(std::string const & filename)
	{
		std::string content = readFile(filename);
		size_t i = 0;
		parse(content, i);
	}

	void Config::save(std::string const & filename) const
	{
		std::string content;
		toString(content);
		writeFile(filename, content);
	}

	std::optional<Config const &> Config::operator [] (std::string const & key) const
	{
		auto it = children.find(key);
		if (it != children.end())
		{
			return it->second;
		}
		return std::optional<Config const &>();
	}

	std::optional<Config &> Config::operator [] (std::string & key)
	{
		auto it = children.find(key);
		if (it != children.end())
		{
			return it->second;
		}
		return std::optional<Config &>();
	}

	std::optional<Config const &> Config::operator [] (int i) const
	{
		auto it = children.find(std::to_string(i));
		if (it != children.end())
		{
			return it->second;
		}
		return std::optional<Config const &>();
	}

	std::optional<Config &> Config::operator [] (int i)
	{
		auto it = children.find(std::to_string(i));
		if (it != children.end())
		{
			return it->second;
		}
		return std::optional<Config &>();
	}

	void Config::parse(std::string const & content, size_t & i)
	{
		type = String;
		text.clear();
		children.clear();
		unsigned int c = 0;

		skipWhiteSpace(content, i);

		// Find out what type of value this is.
		c = getChar(content, i);

		if (c == '{') // Dictionary
		{
			type = Dictionary;
			while (i < content.size())
			{
				// Get Key
				std::string key;
				skipWhiteSpace(content, i);
				if (isStringNext(content, i, "}"))
				{
					i += 1;
					break;
				}
				
				key = trim(readUntilAny(content, i, " \t\n\r:"));

				children[key].parse(content, i);
			}
		}
		else if (c == '[') // List
		{
			type = List;
			int count = 0;
			while (i < content.size())
			{
				skipWhiteSpace(content, i);
				if (isStringNext(content, i, "]"))
				{
					i += 1;
					break;
				}
				std::string key = std::to_string(count);
				children[key].parse(content, i);
				count++;
			}
		}
		else // String
		{
			type = String;
			auto nextC = getChar(content, i);

			std::string quoteStart;
			if (nextC == '"' || nextC == '\'' || nextC == '`') // Quote, grab all of same characters.
			{
				auto quoteC = nextC;
				do
				{
					quoteStart += getStringFromChar(nextC);
					nextC = getChar(content, i);
				} while (nextC == quoteC);
			}
			text += getStringFromChar(nextC);
			if (!quoteStart.empty())
			{
				text = readUntil(content, i, quoteStart);
			}
			else
			{
				text = readUntilAny(content, i, " \t\n\r}]");
			}
		}
	}

	void Config::toString(std::string & t, size_t tabDepth) const
	{
		if (type == Dictionary)
		{
			t += "{\n";
			for (auto iter : children)
			{
				for (unsigned int i = 0; i < tabDepth + 1; i++)
				{
					t += '\t';
				}
				t += iter.first;
				t += " : ";
				iter.second.toString(t, tabDepth + 1);
			}
			for (unsigned int i = 0; i < tabDepth; i++)
			{
				t += '\t';
			}
			t += "}\n";
		}
		else if (type == List)
		{
			t += "[\n";
			unsigned int count = 0;
			while (true)
			{
				auto iter = children.find(std::to_string(count));
				if (iter == children.end())
				{
					break;
				}
				for (unsigned int i = 0; i < tabDepth + 1; i++)
				{
					t += '\t';
				}
				iter->second.toString(t, tabDepth + 1);
				count++;
			}
			for (unsigned int i = 0; i < tabDepth; i++)
			{
				t += '\t';
			}
			t += "]\n";
		}
		else if (type == String)
		{
			t += text;
			t += '\n';
		}
	}

}