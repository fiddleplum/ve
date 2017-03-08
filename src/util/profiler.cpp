#include "profiler.hpp"
#include <SDL.h>
#include <fstream>
#include <stack>

namespace ve
{
	struct Sample
	{
		std::string name;
		float startTime;
	};

	std::stack<Sample> samples;
	std::ofstream profileLog;

	float getSeconds()
	{
		return SDL_GetTicks() / 1000.0f;
	}

	void Profiler::initialize()
	{
		profileLog.open("profile.txt");
		profileLog.precision(5);
		profileLog << std::fixed;
	}

	void Profiler::finalize()
	{
		profileLog.close();
	}

	bool Profiler::isInitialized()
	{
		return profileLog.is_open();
	}

	void Profiler::startSample(std::string const & name)
	{
		if (samples.size() > 256)
		{
			throw std::runtime_error("Profiler start sample without end sample. ");
		}
		std::string spaces = " ";
		for (int i = 0; i < samples.size(); i++)
		{
			spaces += " ";
		}
		samples.push({spaces + name, getSeconds()});
		profileLog << getSeconds() << samples.top().name << " start" << std::endl;
	}

	void Profiler::endSample()
	{
		if(samples.size() == 0)
		{
			throw std::runtime_error("Profiler end sample without start sample. ");
		}
		Sample sample = samples.top();
		samples.pop();
		profileLog << getSeconds() << sample.name << " " << (getSeconds() - sample.startTime) << std::endl;
	}
}