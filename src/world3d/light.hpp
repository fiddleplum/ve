#pragma once

#include "entity.hpp"

namespace ve
{
	namespace world3d
	{
		class Light : public Entity
		{
		public:
			// Constructs as a white light.
			Light();

			// Returns the color.
			Vector3f getColor() const;

			// Sets the color.
			void setColor(Vector3f color);

		private:
			Vector3f color;
		};
	}
}

