#include "log.hpp"
#include <SDL.h>
#include <fstream>

namespace ve
{
	std::ofstream logFile;

	void Log::initialize()
	{
		logFile.open("log.txt");
	}

	void Log::finalize()
	{
		logFile.close();
	}

	bool Log::isInitialized()
	{
		return logFile.is_open();
	}

	void Log::write(std::string const & message)
	{
		logFile << (SDL_GetTicks() / 1000.0f) << " " << message << std::endl;
		logFile.flush();
	}
}