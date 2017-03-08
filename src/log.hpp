#pragma once

#include <string>

namespace ve
{
	class Log
	{
	public:
		//! Initialize the log.
		static void initialize();

		//! Finalize the log.
		static void finalize();

		//! Returns true if the log is initialized.
		static bool isInitialized();

		//! Logs a message to a file.
		static void write(std::string const & message);
	};
}