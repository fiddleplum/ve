#pragma once

#include "image.h"
#include "mesh.h"
#include "ptr.h"
#include <string>

namespace ve
{
	class ResourceStore
	{
	public:
		// Get an image from the store.
		virtual UsePtr<Image> getImage(std::string const & name) const = 0;

		// Load an image into the store with the given name.
		virtual UsePtr<Image> loadImage(std::string const & name, std::string const & location) = 0;

		// Create a new image in the store with the given name.
		virtual UsePtr<Image> createImage(std::string const & name, Vector2i size, Image::Format format) = 0;

		// List the names of all the images in the store.
		virtual std::vector<std::string> listImages() const = 0;

		// Get a mesh from the store.
		virtual UsePtr<Mesh> getMesh(std::string const & name) const = 0;

		// Load a mesh into the store with the given name.
		virtual UsePtr<Mesh> loadMesh(std::string const & name, std::string const & location) = 0;

		// Create a new mesh in the store with the given name.
		virtual UsePtr<Mesh> createMesh(std::string const & name) = 0;

		// List the names of all the meshes in the store.
		virtual std::vector<std::string> listMeshes() const = 0;

	protected:
		// Virtual destructor.
		virtual ~ResourceStore() = default;
	};
}