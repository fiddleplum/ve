#include "image_internal.h"
#include <SDL_image.h>

namespace ve
{
	ImageInternal::ImageInternal(Coord2i size_, Format format_)
	{
		size = size_;
		format = format_;

		switch (format)
		{
			case RGB24:
				bytesPerPixel = 3;
				break;
			case RGBA32:
			case GRAYSCALE32:
				bytesPerPixel = 4;
				break;
		}

		pixels.resize(size[0] * size[1] * bytesPerPixel);
	}

	ImageInternal::ImageInternal(std::string const & filename)
	{
		SDL_Surface * surface = IMG_Load(filename.c_str());
		if (surface == 0)
		{
			throw std::runtime_error("Could not load image '" + filename + "': " + IMG_GetError());
		}
		size[0] = surface->w;
		size[1] = surface->h;

		switch (surface->format->BitsPerPixel)
		{
			case 24:
				format = RGB24;
				bytesPerPixel = 3;
				break;
			case 32:
				format = RGBA32;
				bytesPerPixel = 4;
				break;
			default:
				throw std::runtime_error("Error loading image '" + filename + "': Only RGB24 and RGBA32 pixel formats are supported. ");
		}

		pixels.resize(size[0] * size[1] * bytesPerPixel);
		memcpy(&pixels[0], surface->pixels, pixels.size());
		SDL_FreeSurface(surface);
	}

	void ImageInternal::save(std::string const & filename) const
	{
		if (format != RGB24 && format != RGBA32)
		{
			throw std::runtime_error("Error saving image '" + filename + "': Only RGB24 and RGBA32 pixel formats are supported. ");
		}

		SDL_Surface * surface = SDL_CreateRGBSurface(0, size[0], size[1], bytesPerPixel * 8, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		memcpy(surface->pixels, &pixels[0], surface->pitch * surface->h);
		int result = IMG_SavePNG(surface, filename.c_str());
		SDL_FreeSurface(surface);
		if(result != 0)
		{
			throw std::runtime_error("Error saving image '" + filename + "': File save failed. ");
		}
	}

	Coord2i ImageInternal::getSize() const
	{
		return size;
	}

	ImageInternal::Format ImageInternal::getFormat() const
	{
		return format;
	}

	std::vector<uint8_t> const & ImageInternal::getPixels() const
	{
		return pixels;
	}

	Coord<3, uint8_t> ImageInternal::getPixelRGB(Coord2i position) const
	{
		int offset = size[0] * bytesPerPixel * position[1] + position[0];
		if (format != RGB24)
		{
			throw std::runtime_error("Wrong format for getting the pixel.");
		}
		if (offset < 0 || pixels.size() < offset + 3)
		{
			throw std::runtime_error("When accessing pixel, position is out of bounds.");
		}
		return {pixels[offset + 0], pixels[offset + 1], pixels[offset + 2]};
	}

	Coord<4, uint8_t> ImageInternal::getPixelRGBA(Coord2i position) const
	{
		int offset = size[0] * bytesPerPixel * position[1] + position[0];
		if (format != RGBA32)
		{
			throw std::runtime_error("Wrong format for getting the pixel.");
		}
		if (offset < 0 || pixels.size() < offset + 4)
		{
			throw std::runtime_error("When accessing pixel, position is out of bounds.");
		}
		return {pixels[offset + 0], pixels[offset + 1], pixels[offset + 2], pixels[offset + 3]};
	}

	uint32_t ImageInternal::getPixelGrayScale32(Coord2i position) const
	{
		int offset = size[0] * bytesPerPixel * position[1] + position[0];
		if (format != GRAYSCALE32)
		{
			throw std::runtime_error("Wrong format for getting the pixel.");
		}
		if (offset < 0 || pixels.size() < offset + 4)
		{
			throw std::runtime_error("When accessing pixel, position is out of bounds.");
		}
		return (pixels[offset + 0] << 24) + (pixels[offset + 1] << 16) + (pixels[offset + 2] << 8) + pixels[offset + 3];
	}

	void ImageInternal::setPixelRGB24(Coord2i position, Coord<3, uint8_t> value)
	{
		int offset = size[0] * bytesPerPixel * position[1] + position[0];
		if (format != RGB24)
		{
			throw std::runtime_error("Wrong format for setting the pixel.");
		}
		if (offset < 0 || pixels.size() < offset + 3)
		{
			throw std::runtime_error("When accessing pixel, position is out of bounds.");
		}
		pixels[offset + 0] = value[0];
		pixels[offset + 1] = value[1];
		pixels[offset + 2] = value[2];
	}

	void ImageInternal::setPixelRGBA32(Coord2i position, Coord<4, uint8_t> value)
	{
		int offset = size[0] * bytesPerPixel * position[1] + position[0];
		if (format != RGBA32)
		{
			throw std::runtime_error("Wrong format for setting the pixel.");
		}
		if (offset < 0 || pixels.size() < offset + 4)
		{
			throw std::runtime_error("When accessing pixel, position is out of bounds.");
		}
		pixels[offset + 0] = value[0];
		pixels[offset + 1] = value[1];
		pixels[offset + 2] = value[2];
		pixels[offset + 3] = value[3];
	}

	void ImageInternal::setPixelGrayScale32(Coord2i position, uint32_t value)
	{
		int offset = size[0] * bytesPerPixel * position[1] + position[0];
		if (format != GRAYSCALE32)
		{
			throw std::runtime_error("Wrong format for setting the pixel.");
		}
		if (offset < 0 || pixels.size() < offset + 4)
		{
			throw std::runtime_error("When accessing pixel, position is out of bounds.");
		}
		pixels[offset + 0] = (value >> 24) & 0xff;
		pixels[offset + 1] = (value >> 16) & 0xff;
		pixels[offset + 2] = (value >> 8) & 0xff;
		pixels[offset + 3] = value & 0xff;
	}
}