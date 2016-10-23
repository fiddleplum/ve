#pragma once

#include "coord.h"
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
		virtual Coord2i getSize() const = 0;

		// Get the format of the image.
		virtual Format getFormat() const = 0;

		// Get the raw pixel data.
		virtual std::vector<uint8_t> const & getPixels() const = 0;

		// Read a pixel.
		virtual Coord<3, uint8_t> getPixelRGB(Coord2i position) const = 0;

		// Read a pixel.
		virtual Coord<4, uint8_t> getPixelRGBA(Coord2i position) const = 0;

		// Read a pixel.
		virtual uint32_t getPixelGrayScale32(Coord2i position) const = 0;

		// Write a pixel.
		virtual void setPixelRGB24(Coord2i position, Coord<3, uint8_t> value) = 0;

		// Write a pixel.
		virtual void setPixelRGBA32(Coord2i position, Coord<4, uint8_t> value) = 0;

		// Write a pixel.
		virtual void setPixelGrayScale32(Coord2i position, uint32_t value) = 0;

		// Virtual destructor.
		virtual ~Image() = default;
	};
}