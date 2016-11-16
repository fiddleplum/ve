#pragma once

#include "util/vector.h"
#include <string>
#include <vector>

namespace ve
{
	class Image
	{
	public:
		// The formats supported.
		enum Format { RGB24, RGBA32, GRAYSCALE32 };

		// Saves the image to a file.
		virtual void save(std::string const & filename) const = 0;

		// Get the size of the image.
		virtual Vector2i getSize() const = 0;

		// Get the format of the image.
		virtual Format getFormat() const = 0;

		// Get the raw pixel data.
		virtual std::vector<uint8_t> const & getPixels() const = 0;

		// Read a pixel.
		virtual Vector<3, uint8_t> getPixelRGB(Vector2i position) const = 0;

		// Read a pixel.
		virtual Vector<4, uint8_t> getPixelRGBA(Vector2i position) const = 0;

		// Read a pixel.
		virtual uint32_t getPixelGrayScale32(Vector2i position) const = 0;

		// Write a pixel.
		virtual void setPixelRGB24(Vector2i position, Vector<3, uint8_t> value) = 0;

		// Write a pixel.
		virtual void setPixelRGBA32(Vector2i position, Vector<4, uint8_t> value) = 0;

		// Write a pixel.
		virtual void setPixelGrayScale32(Vector2i position, uint32_t value) = 0;

	protected:
		// Virtual destructor.
		virtual ~Image() = default;
	};
}