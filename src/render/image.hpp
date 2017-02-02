#pragma once

#include "util/vector.hpp"
#include "util/rect.hpp"
#include <string>
#include <vector>

namespace ve
{
	namespace render
	{
		class Image final
		{
		public:
			// The formats supported.
			enum Format { RGB24, RGBA32, GRAYSCALE32 };

			// Create a blank image. Pixels are uninitializaed.
			Image(Vector2i size, Format format);

			// Load a PNG or JPG from a file.
			Image(std::string const & filename);

			// Load from an SDL Surface.
			//Image(void const * sdlSurface);

			// Destructor.
			~Image();

			// Saves the image to a file.
			void save(std::string const & filename) const;

			// Gets the size of the image.
			Vector2i getSize() const;

			// Sets the size of the image. Uninitializes the pixels.
			void setSize(Vector2i size);

			// Gets the format of the image.
			Format getFormat() const;

			// Gets the raw pixel data.
			std::vector<uint8_t> const & getPixels() const;

			// Sets the raw pixel data.
			void setPixels(std::vector<uint8_t> const & pixels);

			// Internal to renderer. Activates the texture in the GL slot.
			void activate(unsigned int slot) const;

			// Internal to renderer. Deactivates all slots equal to or greater than the GL slot. Used to clear out excess textures not needed by a material.
			static void deactivateRest(unsigned int slot);

			// Internal to renderer. Attaches the texture to the currently bound GL frame buffer. Keep in mind on Windows the texture will be rendered upside-down.
			void attachToFrameBuffer(unsigned int attachment);

		private:
			void initializeGLPixels();
			void updateGLPixels() const;
			void loadFromSDLSurface(void const * sdlSurface);

			Format format;
			Vector2i size;
			int bytesPerPixel;
			std::vector<uint8_t> pixels;
			unsigned int glId;
			unsigned int glFormat;
			unsigned int glType;
		};
	}
}