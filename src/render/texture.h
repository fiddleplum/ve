#pragma once

#include "../image.h"

namespace ve
{
	namespace render
	{
		class Texture
		{
		public:
			// Creates a new texture from a size and format (from Image::Format). The pixels are uninitialized.
			Texture(Vector2i size, Image::Format format);

			// Creates a new texture from an Image.
			Texture(UsePtr<Image> image);

			// Creates a texture from a file.
			Texture(std::string const & filename);

			// Destroys the texture.
			virtual ~Texture();

			// Returns the size of the texture.
			Vector2i getSize() const;

			// Updates the pixels from the image.
			void updatePixels(UsePtr<Image> image);

			// Activates the texture in the GL slot. Used by Models.
			void activate(unsigned int slot) const;

			// Deactivates all slots equal to or greater than the GL slot. Used to clear out excess textures not needed by a material.
			static void deactivateRest(unsigned int slot);

		private:
			void setPixels(Vector2i size, Image::Format format, const uint8_t * pixels);

			Vector2i size;
			unsigned int id;

			friend class TextureStage; // TextureStage needs the id.
		};
	}
}

