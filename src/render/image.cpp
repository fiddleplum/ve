#include "render/image.hpp"
#include "render/open_gl.hpp"
#include "util/math.hpp"
#include <SDL_image.h>

namespace ve
{
	namespace render
	{
		std::vector<unsigned int> boundGLTextureIds; // Currently bound GL Texture IDs.

		Image::Image(Vector2i size_, Format format_)
		{
			format = format_;
			size = size_;

			switch (format)
			{
				case RGB24:
				case DEPTH:
					bytesPerPixel = 3;
					break;
				case RGBA32:
				case GRAYSCALE32:
					bytesPerPixel = 4;
					break;
			}

			pixels.resize(size[0] * size[1] * bytesPerPixel);

			glGenTextures(1, &glId);
			initializeGLPixels();
		}

		Image::Image(std::string const & filename)
		{
			SDL_Surface * surface = IMG_Load(filename.c_str());
			if (surface == 0)
			{
				throw std::runtime_error("Error loading image '" + filename + "'. " + IMG_GetError());
			}
			try
			{
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
						throw std::runtime_error("Error loading image '" + filename + "'. Only RGB24 and RGBA32 pixel formats are supported. ");
				}

				pixels.resize(size[0] * size[1] * bytesPerPixel);
				memcpy(&pixels[0], surface->pixels, pixels.size());

				glGenTextures(1, &glId);
				initializeGLPixels();
			}
			catch (std::runtime_error const & e)
			{
				SDL_FreeSurface(surface);
				throw std::runtime_error("Error loading image '" + filename + "'. " + e.what());
			}
			SDL_FreeSurface(surface);
		}

		Image::~Image()
		{
			glDeleteTextures(1, &glId);
		}

		void Image::save(std::string const & filename) const
		{
			if (format != RGB24 && format != RGBA32)
			{
				throw std::runtime_error("Error saving image '" + filename + "'. Only RGB24 and RGBA32 pixel formats are currently supported. ");
			}

			SDL_Surface * surface = SDL_CreateRGBSurface(0, size[0], size[1], bytesPerPixel * 8, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
			memcpy(surface->pixels, &pixels[0], surface->pitch * surface->h);
			int result = IMG_SavePNG(surface, filename.c_str());
			SDL_FreeSurface(surface);
			if (result != 0)
			{
				throw std::runtime_error("Error saving image '" + filename + "'. File save failed. ");
			}
		}

		Vector2i Image::getSize() const
		{
			return size;
		}

		void Image::setSize(Vector2i size_)
		{
			size = size_;
			pixels.resize(size[0] * size[1] * bytesPerPixel);
			initializeGLPixels();
		}

		Image::Format Image::getFormat() const
		{
			return format;
		}

		std::vector<uint8_t> const & Image::getPixels() const
		{
			return pixels;
		}

		void Image::setPixels(std::vector<uint8_t> const & pixels_)
		{
			if (pixels.size() != pixels_.size())
			{
				throw std::runtime_error("Error setting pixels. Wrong size for pixel data. ");
			}
			pixels = pixels_;
			updateGLPixels();
		}

		void Image::activate(unsigned int slot) const
		{
			if (slot >= boundGLTextureIds.size() || glId != boundGLTextureIds[slot])
			{
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_2D, glId);
				if (slot >= boundGLTextureIds.size())
				{
					boundGLTextureIds.resize(slot + 1);
				}
				boundGLTextureIds[slot] = glId;
			}
		}

		void Image::deactivateRest(unsigned int slot)
		{
			for (; slot < boundGLTextureIds.size(); slot++)
			{
				if (boundGLTextureIds[slot] == 0)
				{
					break; // If this one is zero, the rest are zero.
				}
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_2D, 0);
				boundGLTextureIds[slot] = 0;
			}
		}

		void Image::attachToFrameBuffer(unsigned int attachment)
		{
			glFramebufferTexture(GL_FRAMEBUFFER, attachment, glId, 0);
		}

		void Image::initializeGLPixels()
		{
			int glInternalFormat;
			switch (format)
			{
				case Image::RGB24:
					glInternalFormat = GL_RGB8;
					glFormat = GL_RGB;
					glType = GL_UNSIGNED_BYTE;
					break;
				case Image::RGBA32:
					glInternalFormat = GL_RGBA8;
					glFormat = GL_RGBA;
					glType = GL_UNSIGNED_BYTE;
					break;
				case Image::GRAYSCALE32:
					glInternalFormat = GL_R32UI;
					glFormat = GL_RED;
					glType = GL_UNSIGNED_INT;
					break;
				case Image::DEPTH:
					glInternalFormat = GL_DEPTH_COMPONENT24;
					glFormat = GL_DEPTH_COMPONENT;
					glType = GL_UNSIGNED_BYTE;
					break;
			}

			glBindTexture(GL_TEXTURE_2D, glId);
			if (pixels.size() > 0)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, size[0], size[1], 0, glFormat, glType, &pixels[0]);
				int level = 1;
				while (math::max(size[0] >> level, size[1] >> level) > 0)
				{
					glTexImage2D(GL_TEXTURE_2D, level, glInternalFormat, size[0] >> level, size[1] >> level, 0, glFormat, glType, 0);
					level++;
				}
			}
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		void Image::updateGLPixels() const
		{
			glBindTexture(GL_TEXTURE_2D, glId);
			if (pixels.size() > 0)
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size[0], size[1], glFormat, glType, &pixels[0]);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
}