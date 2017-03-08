#include "util/config.hpp"
#include "util/stringutil.hpp"

namespace ve
{
	Config::Config()
	{
	}

	Config::Config(std::string const & filename)
	{
		load(filename);
	}

	void Config::load(std::string const & filename)
	{
		// Clear out data.
		type = String;
		text.clear();
		children.clear();

		// Load new data.
		std::string content = readFile(filename);
		size_t i = 0;
		parse(content, i);
	}

	void Config::save(std::string const & filename) const
	{
		std::string content;
		stringify(content);
		writeFile(filename, content);
	}

	void Config::stringify(std::string & t, size_t tabDepth) const
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
				iter.second.stringify(t, tabDepth + 1);
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
				iter->second.stringify(t, tabDepth + 1);
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

	bool Config::hasChild(std::string const & key) const
	{
		return children.find(key) != children.end();
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

	std::optional<Config const &> Config::operator [] (int index) const
	{
		auto it = children.find(std::to_string(index));
		if (it != children.end())
		{
			return it->second;
		}
		return std::optional<Config const &>();
	}

	std::optional<Config &> Config::operator [] (int index)
	{
		auto it = children.find(std::to_string(index));
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

				skipWhiteSpace(content, i);
				c = getChar(content, i);
				if (c != ':')
				{
					throw std::runtime_error("Expected colon after character");
				}

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
			std::string quoteStart;
			if (c == '"' || c == '\'' || c == '`') // Quote, grab all of same characters.
			{
				auto quoteC = c;
				do
				{
					quoteStart += getStringFromChar(c);
					c = getChar(content, i);
				} while (c == quoteC);
			}
			text += getStringFromChar(c);
			if (!quoteStart.empty())
			{
				text += readUntil(content, i, quoteStart);
				i += quoteStart.size();
			}
			else
			{
				text += readUntilAny(content, i, " \t\n\r}]");
			}
		}
	}

	template <> bool Config::getChildAs(std::string const & key, bool defaultValue) const
	{
		auto it = children.find(key);
		if (it != children.end() && it->second.type == String)
		{
			if (it->second.text == "true") return true;
			if (it->second.text == "false") return false;
		}
		return defaultValue;
	}

	template <> int Config::getChildAs(std::string const & key, int defaultValue) const
	{
		auto it = children.find(key);
		if (it != children.end() && it->second.type == String)
		{
			try
			{
				return std::stoi(it->second.text);
			}
			catch (...) {}
		}
		return defaultValue;
	}

	template <> float Config::getChildAs(std::string const & key, float defaultValue) const
	{
		auto it = children.find(key);
		if (it != children.end() && it->second.type == String)
		{
			try
			{
				return std::stof(it->second.text);
			}
			catch (...) {}
		}
		return defaultValue;
	}

	template <> std::string Config::getChildAs(std::string const & key, std::string defaultValue) const
	{
		auto it = children.find(key);
		if (it != children.end() && it->second.type == String)
		{
			return it->second.text;
		}
		return defaultValue;
	}

	template <> bool Config::getChildAs(int index, bool defaultValue) const
	{
		auto it = children.find(std::to_string(index));
		if (it != children.end() && it->second.type == String)
		{
			if (it->second.text == "true") return true;
			if (it->second.text == "false") return false;
		}
		return defaultValue;
	}

	template <> int Config::getChildAs(int index, int defaultValue) const
	{
		auto it = children.find(std::to_string(index));
		if (it != children.end() && it->second.type == String)
		{
			try
			{
				return std::stoi(it->second.text);
			}
			catch (...) {}
		}
		return defaultValue;
	}

	template <> float Config::getChildAs(int index, float defaultValue) const
	{
		auto it = children.find(std::to_string(index));
		if (it != children.end() && it->second.type == String)
		{
			try
			{
				return std::stof(it->second.text);
			}
			catch (...) {}
		}
		return defaultValue;
	}

	template <> std::string Config::getChildAs(int index, std::string defaultValue) const
	{
		auto it = children.find(std::to_string(index));
		if (it != children.end() && it->second.type == String)
		{
			return it->second.text;
		}
		return defaultValue;
	}
}