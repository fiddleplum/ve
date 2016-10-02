#pragma once

#include <string>
#include <map>
#include "std/optional.hpp"

namespace ve
{
	// The tree node structure config. File format below.
	class Config
	{
	public:
		// Default constructor.
		Config();

		// Construct from a file.
		Config(std::string const & filename);

		// Save a value to a file.
		void save(std::string const & filename) const;

		// Return a string of the config.
		void toString(std::string & text, size_t tabDepth = 0) const;

		enum Type { Dictionary, List, String };

		std::optional<Config const &> operator [] (std::string const & key) const;

		std::optional<Config &> operator [] (std::string & key);

		std::optional<Config const &> operator [] (int i) const;

		std::optional<Config &> operator [] (int i);

		// The type of the value.
		Type type = String;

		// The text of the string, if it is a string.
		std::string text;

		// The children if it is a dictionary or list.
		std::map<std::string, Config> children;

	private:
		void parse(std::string const & text, size_t & i);
	};
}

/*
Config is a format similar but simpler than JSON. The syntax is as follows.
Root = > Value
Value = > Dictionary | List | String
Dictionary = > '{' (Key ':' Value)* '}'
List = > '[' Value* ']'
String = > string ended by whitespace, ] (if in array), or } (if in dictionary) or enclosed by " or ```
Key = > string ended by ':' or whitespace
All white space is trimmed unless enclosed by " or ```
*/