#pragma once

#include "input_event.hpp"

namespace ve
{
	namespace world
	{
		class Controller
		{
		public:
			virtual void update(float dt) = 0;

			virtual void handleInputEvent(InputEvent const & inputEvent) = 0;
		};
	}
}