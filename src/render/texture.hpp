#pragma once

#include "util/image.hpp"
#include "util/vector.hpp"
#include "util/ptr.hpp"

namespace ve
{
	class Texture
	{
	public:
		// Creates a blank texture of the given size and format.
		Texture(Vector2i size, Image::Format format);

		// Creates a new texture from an Image.
		Texture(Ptr<Image> image);

		// Destroys the texture.
		virtual ~Texture();

		// Returns the size of the texture.
		Vector2i getSize() const;

		// Returns the format of the texture. 
		Image::Format getFormat() const;

		// Sets the size of the texture. Invalidates the pixels.
		void setSize(Vector2i size);

		// Sets the format. Invalidates the pixels.
		void setFormat(Image::Format format);

		// Sets the texture from the image.
		void set(Ptr<Image> const & image);

		// Activates the texture in the GL slot. Used by Models.
		void activate(unsigned int slot) const;

		// Deactivates all slots equal to or greater than the GL slot. Used to clear out excess textures not needed by a material.
		static void deactivateRest(unsigned int slot);

		// Returns the OpenGL ID.
		unsigned int getGLId() const;

	private:
		void set(Vector2i size, Image::Format format, uint8_t const * pixels);

		unsigned int id;
		Vector2i size;
		Image::Format format;
	};
}