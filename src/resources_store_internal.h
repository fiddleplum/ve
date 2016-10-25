#pragma once

#include "resource_store.h"
#include "image_internal.h"
#include "render/texture.h"
#include "mesh_internal.h"
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

		// Get a mesh from the store.
		virtual UsePtr<Mesh> getMesh(std::string const & name) = 0;

		// Get a mesh from the store.
		virtual UsePtr<MeshInternal> getMeshInternal(std::string const & name) = 0;

		// Load a mesh into the store with the given name.
		virtual void loadMesh(std::string const & name, std::string const & location) = 0;

		// Create a new mesh in the store with the given name.
		virtual void createMesh(std::string const & name) = 0;

		// List the names of all the meshes in the store.
		virtual std::vector<std::string> listMeshes() const = 0;

	private:
		Cache<ImageInternal> imageCache;
		Cache<Texture> textureCache;
		Cache<MeshInternal> meshCache;
	};
}