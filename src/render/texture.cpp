#include "texture.hpp"
#include "open_gl.hpp"

namespace ve
{
	std::vector<unsigned int> currentTextures; // Current Textures in the OpenGL state.

	Texture::Texture(Ptr<Image> image)
	{
		glGenTextures(1, &id);
		setPixels(image->getSize(), image->getFormat(), &image->getPixels()[0]);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	Vector2i Texture::getSize() const
	{
		return size;
	}

	void Texture::updatePixels(Ptr<Image> image)
	{
		setPixels(image->getSize(), image->getFormat(), &image->getPixels()[0]);
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

	unsigned int Texture::getGLId() const
	{
		return id;
	}

	void Texture::setPixels(Vector2i size_, Image::Format format, uint8_t const * pixels)
	{
		size = size_;

		int glInternalFormat;
		int glFormat;
		int glType;
		switch (format)
		{
			case Image::RGB24:
				glInternalFormat = GL_RGB;
				glFormat = GL_RGB;
				glType = GL_UNSIGNED_BYTE;
				break;
			case Image::RGBA32:
				glInternalFormat = GL_RGBA;
				glFormat = GL_RGBA;
				glType = GL_UNSIGNED_BYTE;
				break;
			case Image::GRAYSCALE32:
				glInternalFormat = GL_R32UI;
				glFormat = GL_RED;
				glType = GL_UNSIGNED_INT;
				break;
		}

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, size[0], size[1], 0, glFormat, glType, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

