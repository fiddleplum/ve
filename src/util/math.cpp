#include "math.hpp"
#include <cstdlib>
#include <ctime>
#ifndef _MSC_VER
#include <float.h>
#endif
#include <exception>

namespace ve
{
	namespace math
	{
		bool firstRandom = true;

		int random(int min, int max)
		{
			if (max < min || max - min > RAND_MAX)
			{
				throw std::exception();
			}
			if (min == max)
			{
				return min;
			}
			if (firstRandom)
			{
				std::srand((int)(std::time(nullptr)));
				firstRandom = false;
			}
			return (rand() % (max - min)) + min;
		}

		float random(float min, float max)
		{
			if (max < min)
			{
				throw std::exception();
			}
			if (firstRandom)
			{
				std::srand((int)(std::time(nullptr)));
				firstRandom = false;
			}
			return (float)rand() * ((max - min) / (float)RAND_MAX) + min;
		}

		int ceil(int i, int period)
		{
			if (period <= 0)
			{
				throw std::exception();
			}
			return period * (i > 0) ? (1 + (i - 1) / period) : (i / period);
		}
	}
}