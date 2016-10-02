#include "vector.h"
#include "cache.h"

namespace ve
{
	class Image
	{
	public:
		// The formats supported.
		enum Format { RGB24, RGBA32, GRAYSCALE32 };

		// Create a blank image.
		Image(Vector2i size, Format format);

		// Load a PNG or JPG from a file.
		Image(std::string const & filename);

		// Saves the image to a file.
		void save(std::string const & filename);

		// Get the size of the image.
		Vector2i getSize() const;

		// Get the format of the image.
		Format getFormat() const;

		// Get the raw pixel data.
		std::vector<uint8_t> const & getPixels() const;

		// Read a pixel.
		Vector<3, uint8_t> getPixelRGB(Vector2i position) const;

		// Read a pixel.
		Vector<4, uint8_t> getPixelRGBA(Vector2i position) const;

		// Read a pixel.
		uint32_t getPixelGrayScale32(Vector2i position) const;

		// Write a pixel.
		void setPixelRGB24(Vector2i position, Vector<3, uint8_t> value);

		// Write a pixel.
		void setPixelRGBA32(Vector2i position, Vector<4, uint8_t> value);

		// Write a pixel.
		void setPixelGrayScale32(Vector2i position, uint32_t value);

		// Cache for images.
		static Cache<Image> cache;

	private:
		Format format;
		int bytesPerPixel;
		Vector2i size;
		std::vector<uint8_t> pixels;
	};
}

