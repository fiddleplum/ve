#pragma once

#include "entity.h"

namespace ve
{
	namespace scene
	{
		class Light : public Entity
		{
		public:
			Light();

			Vector3f getColor() const;

			void setColor(Vector3f color);

		private:
			Vector3f color;
		};
	}
}

