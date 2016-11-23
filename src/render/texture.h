#pragma once

#include "render/image.h"
#include "util/vector.h"
#include "util/ptr.h"
#include <SDL.h>

namespace ve
{
	class Texture
	{
	public:
		// Creates a new texture from an Image.
		Texture(Ptr<Image> image);

		// Destroys the texture.
		virtual ~Texture();

		// Returns the size of the texture.
		Vector2i getSize() const;

		// Updates the pixels from the image.
		void updatePixels(Ptr<Image> image);

		// Activates the texture in the GL slot. Used by Models.
		void activate(unsigned int slot) const;

		// Deactivates all slots equal to or greater than the GL slot. Used to clear out excess textures not needed by a material.
		static void deactivateRest(unsigned int slot);

		// Returns the OpenGL ID.
		unsigned int getGLId() const;

	private:
		void setPixels(Vector2i size, Image::Format format, uint8_t const * pixels);

		unsigned int id;
		Vector2i size;
	};
}