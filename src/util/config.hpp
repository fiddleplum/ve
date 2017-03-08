#pragma once

#include <string>
#include <map>
#include "std/optional.hpp"

namespace ve
{
	/*! The tree node structure config.
	Config is a format similar but simpler than JSON. The syntax is as follows.
	Root = > Value
	Value = > Dictionary | List | String
	Dictionary = > '{' (Key ':' Value)* '}'
	List = > '[' Value* ']'
	String = > string ended by whitespace, ] (if in array), or } (if in dictionary) or enclosed by consecutive ', `, or " characters
	Key = > string ended by ':' or whitespace
	All white space is trimmed unless quoted
	*/
	class Config
	{
	public:
		//! Default constructor.
		Config();

		//! Construct from a file.
		Config(std::string const & filename);

		//! Load a value from a file. Erases all previous data.
		void load(std::string const & filename);

		//! Save a value to a file.
		void save(std::string const & filename) const;

		//! Return a string of the config, recursively.
		void stringify(std::string & text, size_t tabDepth = 0) const;

		//! Returns true if the child exists.
		bool hasChild(std::string const & key) const;

		//! Return the child string value as a given type or the default value if the child does not exist, is not a string, or is not convertible to the type.
		template <typename T> T getChildAs(std::string const & key, T defaultValue) const;

		//! Return the child string value as a given type or the default value if the child does not exist, is not a string, or is not convertible to the type.
		template <typename T> T getChildAs(int index, T defaultValue) const;

		//! The possible types of the value.
		enum Type { Dictionary, List, String };

		//! Access the value at the key.
		std::optional<Config const &> operator [] (std::string const & key) const;

		//! Access the value at the key.
		std::optional<Config &> operator [] (std::string & key);

		//! Access the value at the index.
		std::optional<Config const &> operator [] (int index) const;

		//! Access the value at the index.
		std::optional<Config &> operator [] (int index);

		//! The type of the value.
		Type type = String;

		//! The text of the string, if it is a string.
		std::string text;

		//! The children if it is a dictionary or list.
		std::map<std::string, Config> children;

	private:
		void parse(std::string const & text, size_t & i);
	};

	template <> bool Config::getChildAs(std::string const & key, bool defaultValue) const;
	template <> int Config::getChildAs(std::string const & key, int defaultValue) const;
	template <> float Config::getChildAs(std::string const & key, float defaultValue) const;
	template <> std::string Config::getChildAs(std::string const & key, std::string defaultValue) const;
	template <> bool Config::getChildAs(int index, bool defaultValue) const;
	template <> int Config::getChildAs(int index, int defaultValue) const;
	template <> float Config::getChildAs(int index, float defaultValue) const;
	template <> std::string Config::getChildAs(int index, std::string defaultValue) const;
}