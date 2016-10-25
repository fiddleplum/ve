#pragma once

#include "resource_store.h"
#include "image_internal.h"
#include "render/texture.h"
#include "mesh_internal.h"
#include "render/vertex_buffer_object.h"
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
		UsePtr<Mesh> getMesh(std::string const & name) override;

		// Get a mesh from the store.
		UsePtr<MeshInternal> getMeshInternal(std::string const & name);

		// Load a mesh into the store with the given name.
		void loadMesh(std::string const & name, std::string const & location) override;

		// Create a new mesh in the store with the given name.
		void createMesh(std::string const & name) override;

		// List the names of all the meshes in the store.
		std::vector<std::string> listMeshes() const override;

		// Get a vertex buffer object from the store.
		UsePtr<VertexBufferObject> getVertexBufferObject(std::string const & name);

		// Create a vertex buffer object from an mesh in the store.
		void creaateVertexBufferObject(std::string const & name, UsePtr<Mesh> mesh);

	private:
		Cache<ImageInternal> imageCache;
		Cache<Texture> textureCache;
		Cache<MeshInternal> meshCache;
		Cache<VertexBufferObject> vertexBufferObjectCache;
	};
}