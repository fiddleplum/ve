#pragma once

#include <string>

namespace ve
{
	//! A time profiler to analyze how long something takes. In config.txt, use profiler = true to use it.
	class Profiler
	{
	public:
		//! Initializes the profiler. Called by App.
		static void initialize();

		//! Finalizes the profiler. Called by App.
		static void finalize();

		//! Starts a sample with the given name.
		static void startSample(std::string const & name);

		//! Ends a sample with the given name.
		static void endSample();
	};
}