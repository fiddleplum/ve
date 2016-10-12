#pragma once

#include "image.h"
#include "vector.h"
#include "ptr.h"

namespace ve
{
	namespace render
	{
		class Texture
		{
		public:
			// Destroys the texture.
			virtual ~Texture() = default;

			// Returns the size of the texture.
			virtual Vector2i getSize() const = 0;

			// Updates the pixels from the image.
			virtual void updatePixels(UsePtr<Image> image) = 0;
		};
	}
}

