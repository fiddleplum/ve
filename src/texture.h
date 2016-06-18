#pragma once

#include "image.h"

namespace ve
{
	class Texture
	{
	public:
		// Creates a new texture from raw RGBA32 pixels.
		Texture(UsePtr<Image> image);

		// Creates a texture from a file.
		Texture(std::string const & filename);

		// Destroys the texture.
		virtual ~Texture();

		// Returns the size of the texture.
		Vector2i getSize() const;

		// Updates the pixels from the image.
		void updatePixels(UsePtr<Image> image);

		// Sets the texture content from raw RGBA32 pixels.
		void setPixels(void const * pixels);

		// Activates the texture in the GL slot. Used by Models.
		void activate(unsigned int slot) const;

		// Deactivates all slots equal to or greater than the GL slot. Used by Models.
		static void deactivateRest(unsigned int slot);

		// Cache for textures.
		static Cache<Texture> cache;

	private:
		Vector2i size;
		unsigned int id;
	};
}

