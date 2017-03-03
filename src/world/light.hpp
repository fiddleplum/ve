#pragma once

#include "world/entity.hpp"

namespace ve
{
	namespace world
	{
		class Light : public Entity
		{
		public:
			// Constructs as a white light.
			Light();

			// Destructs the light.
			virtual ~Light();

			// Returns the color.
			Vector3f getColor() const;

			// Sets the color.
			void setColor(Vector3f color);

		private:
			Vector3f color;
		};
	}
}

