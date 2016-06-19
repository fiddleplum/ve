#include "texture.h"
#include "open_gl.h"

namespace ve
{
	std::vector<unsigned int> currentTextures; // Current textures in the OpenGL state.

	Texture::Texture(UsePtr<Image> image)
	{
		init(image);
	}

	Texture::Texture(std::string const & filename)
	{
		UsePtr<Image> image = Image::cache.get(filename);
		if (!image.isValid())
		{
			image = Image::cache.create(filename, filename);
		}
		init(image);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	Vector2i Texture::getSize() const
	{
		return size;
	}

	void Texture::updatePixels(UsePtr<Image> image)
	{
		size = image->getSize();

		GLint internalFormat;
		GLenum format;
		GLenum type;
		switch (image->getFormat())
		{
		case Image::RGB24:
			internalFormat = format = GL_RGB; type = GL_UNSIGNED_BYTE; break;
		case Image::RGBA32:
			internalFormat = format = GL_RGBA; type = GL_UNSIGNED_BYTE; break;
		case Image::GRAYSCALE32:
			internalFormat = GL_R32UI; format = GL_RED; type = GL_UNSIGNED_INT; break;
		}

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size[0], size[1], 0, format, type, &image->getPixels()[0]);
	}

	void Texture::activate(unsigned int slot) const
	{
		if (slot >= currentTextures.size() || id != currentTextures[slot])
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, id);
			if (slot >= currentTextures.size())
			{
				currentTextures.resize(slot + 1);
			}
			currentTextures[slot] = id;
		}
	}

	void Texture::deactivateRest(unsigned int slot)
	{
		for (; slot < currentTextures.size(); slot++)
		{
			if (currentTextures[slot] == 0)
			{
				break; // If this one is zero, the rest are zero.
			}
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, 0);
			currentTextures[slot] = 0;
		}
	}

	void Texture::init(UsePtr<Image> image)
	{
		glGenTextures(1, &id);
		updatePixels(image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

