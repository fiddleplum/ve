#pragma once

#include "resource_store.h"
#include "image_internal.h"
#include "texture.h"
#include "cache.h"

namespace ve
{
	class ResourceStoreInternal : public virtual ResourceStore
	{
	public:
		// Get an image from the store.
		UsePtr<Image> getImage(std::string const & name) override;

		// Get an image from the store.
		UsePtr<ImageInternal> getImageInternal(std::string const & name);

		// Load an image into the store with the given name.
		void loadImage(std::string const & name, std::string const & location) override;

		// Create a new image in the store with the given name.
		void createImage(std::string const & name, Coord2i size, Image::Format format) override;

		// List the names of all the images in the store.
		std::vector<std::string> listImages() const override;

		// Get a texture from the store.
		UsePtr<Texture> getTexture(std::string const & name);

		// Create a texture from an image in the store.
		void createTexture(std::string const & name, UsePtr<Image> image);

	private:
		Cache<ImageInternal> imageCache;
		Cache<Texture> textureCache;
	};
}