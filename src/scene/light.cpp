#include "light.h"

namespace ve
{
	namespace scene
	{
		Light::Light()
		{
			color = { 1, 1, 1 };
		}

		Vector3f Light::getColor() const
		{
			return color;
		}

		void Light::setColor(Vector3f color_)
		{
			color = color_;
		}
	}
}

