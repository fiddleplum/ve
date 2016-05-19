#pragma once

#include "vector.h"
#include <vector>

namespace ve
{
	class Display
	{
	public:
		// Returns the total number of displays in the system.
		static int getNumDisplays();

		// Returns the resolution that the display had when the application started.
		static Vector2i getStartingResolution(int display);

		// Returns the resolution that the disply currently has.
		static Vector2i getCurrentResolution(int display);

		// Returns a list of all resolutions that the display is capable of using.
		static std::vector<Vector2i> getAllResolutions(int display);
	};
}

