#pragma once

#include "render/image.h"

namespace ve
{
	class ImageInternal : public virtual Image
	{
	public:
		// Create a blank image. Pixels are uninitializaed.
		ImageInternal(Vector2i size, Format format);

		// Load a PNG or JPG from a file.
		ImageInternal(std::string const & filename);

		// Saves the image to a file.
		void save(std::string const & filename) const override;

		// Get the size of the image.
		Vector2i getSize() const override;

		// Get the format of the image.
		Format getFormat() const override;

		// Get the raw pixel data.
		std::vector<uint8_t> const & getPixels() const override;

		// Read a pixel.
		Vector<3, uint8_t> getPixelRGB(Vector2i position) const override;

		// Read a pixel.
		Vector<4, uint8_t> getPixelRGBA(Vector2i position) const override;

		// Read a pixel.
		uint32_t getPixelGrayScale32(Vector2i position) const override;

		// Write a pixel.
		void setPixelRGB24(Vector2i position, Vector<3, uint8_t> value) override;

		// Write a pixel.
		void setPixelRGBA32(Vector2i position, Vector<4, uint8_t> value) override;

		// Write a pixel.
		void setPixelGrayScale32(Vector2i position, uint32_t value) override;

	private:
		Format format;
		int bytesPerPixel;
		Vector2i size;
		std::vector<uint8_t> pixels;
	};
}