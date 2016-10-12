#pragma once

#include "render/texture.h"

namespace ve
{
	namespace render
	{
		class TextureInternal : public Texture
		{
		public:
			// Creates a new texture from a size and format (from Image::Format). The pixels are uninitialized.
			TextureInternal(Vector2i size, Image::Format format);

			// Creates a new texture from an Image.
			TextureInternal(UsePtr<Image> image);

			// Destroys the texture.
			virtual ~TextureInternal();

			// Returns the size of the texture.
			Vector2i getSize() const override;

			// Updates the pixels from the image.
			void updatePixels(UsePtr<Image> image) override;

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

