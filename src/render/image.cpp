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
			size = size_;
			format = format_;
			glGenTextures(1, &glId);
			initializeGLPixels(nullptr);
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
				size = {surface->w, surface->h};
				switch (surface->format->BitsPerPixel)
				{
					case 24:
						format = RGB24;
						break;
					case 32:
						format = RGBA32;
						break;
					default:
						throw std::runtime_error("Error loading image '" + filename + "'. Only RGB24 and RGBA32 pixel formats are supported. ");
				}

				glGenTextures(1, &glId);
				initializeGLPixels(surface->pixels);
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
			glGetTexImage(GL_TEXTURE_2D, 0, glFormat, glType, surface->pixels);
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
			initializeGLPixels(nullptr);
		}

		Image::Format Image::getFormat() const
		{
			return format;
		}

		std::vector<uint8_t> Image::getPixels() const
		{
			std::vector<uint8_t> pixels;
			pixels.resize(size[0] * size[1] * bytesPerPixel);
			glGetTexImage(GL_TEXTURE_2D, 0, glFormat, glType, &pixels[0]);
			return pixels;
		}

		void Image::setPixels(std::vector<uint8_t> const & pixels)
		{
			if (size[0] * size[1] * bytesPerPixel != pixels.size())
			{
				throw std::runtime_error("Error setting pixels. Wrong size for pixel data. ");
			}
			glBindTexture(GL_TEXTURE_2D, glId);
			if (pixels.size() > 0)
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size[0], size[1], glFormat, glType, &pixels[0]);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

		void Image::initializeGLPixels(void const * pixels)
		{
			switch (format)
			{
				case Image::RGB24:
					glFormat = GL_RGB;
					glType = GL_UNSIGNED_BYTE;
					glInternalFormat = GL_RGB8;
					bytesPerPixel = 3;
					break;
				case Image::RGBA32:
					glFormat = GL_RGBA;
					glType = GL_UNSIGNED_BYTE;
					glInternalFormat = GL_RGBA8;
					bytesPerPixel = 4;
					break;
				case Image::GRAYSCALE32:
					glFormat = GL_RED;
					glType = GL_UNSIGNED_INT;
					glInternalFormat = GL_R32UI;
					bytesPerPixel = 4;
					break;
				case Image::DEPTH:
					glFormat = GL_DEPTH_COMPONENT;
					glType = GL_UNSIGNED_BYTE;
					glInternalFormat = GL_DEPTH_COMPONENT24;
					bytesPerPixel = 1;
					break;
			}

			glBindTexture(GL_TEXTURE_2D, glId);
			if (size[0] > 0 && size[1] > 0)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, size[0], size[1], 0, glFormat, glType, pixels);
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
	}
}