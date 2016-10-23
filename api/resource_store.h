#pragma once

#include "image.h"
#include "ptr.h"
#include <string>

namespace ve
{
	class ResourceStore
	{
	public:
		// Get an image into the store.
		virtual UsePtr<Image> getImage(std::string const & name) = 0;

		// Load an image into the store with the given name.
		virtual void loadImage(std::string const & name, std::string const & location) = 0;

		// Create a new image in the store with the given name.
		virtual void createImage(std::string const & name, Coord2i size, Image::Format format) = 0;

		// List the names of all the images in the store.
		virtual std::vector<std::string> listImages() const = 0;

	protected:
		// Virtual destructor.
		virtual ~ResourceStore() = default;
	};
}