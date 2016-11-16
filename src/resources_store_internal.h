#pragma once

#include "resource_store.h"
#include "render/image_internal.h"
#include "render/texture.h"
#include "render/mesh_internal.h"
#include "render/vertex_buffer_object.h"
#include "render/shader.h"
#include "render/material.h"
#include "util/cache.h"

namespace ve
{
	// Contains objects that reference outside sources such as the GPU or file system.
	class ResourceStoreInternal : public virtual ResourceStore
	{
	public:
		// Get an image from the store.
		UsePtr<Image> getImage(std::string const & name) const override;

		// Get an image from the store.
		UsePtr<ImageInternal> getImageInternal(std::string const & name) const;

		// Load an image into the store with the given name.
		UsePtr<Image> loadImage(std::string const & name, std::string const & location = "") override;

		// Create a new image in the store with the given name.
		UsePtr<Image> createImage(std::string const & name, Vector2i size, Image::Format format) override;

		// List the names of all the images in the store.
		std::vector<std::string> listImages() const override;

		// Get a texture from the store.
		UsePtr<Texture> getTexture(std::string const & name) const;

		// Create a texture from an image in the store.
		UsePtr<Texture> createTexture(std::string const & name, UsePtr<Image> image);

		// Get a mesh from the store.
		UsePtr<Mesh> getMesh(std::string const & name) const override;

		// Get a mesh from the store.
		UsePtr<MeshInternal> getMeshInternal(std::string const & name) const;

		// Load a mesh into the store with the given name.
		UsePtr<Mesh> loadMesh(std::string const & name, std::string const & location = "") override;

		// Create a new mesh in the store with the given name.
		UsePtr<Mesh> createMesh(std::string const & name) override;

		// List the names of all the meshes in the store.
		std::vector<std::string> listMeshes() const override;

		// Get a vertex buffer object from the store.
		UsePtr<VertexBufferObject> getVertexBufferObject(std::string const & name) const;

		// Create a vertex buffer object from an mesh in the store.
		UsePtr<VertexBufferObject> createVertexBufferObject(std::string const & name, UsePtr<Mesh> mesh = UsePtr<Mesh>());

		// List the names of all the vertex buffer objects in the store.
		std::vector<std::string> listVertexBufferObjects() const;

		// Get a shader from the store.
		UsePtr<Shader> getShader(std::string const & name) const;

		// Load a shader from a config.
		UsePtr<Shader> loadShader(std::string const & name, Config const & config);

		// Load a shader from a file.
		UsePtr<Shader> loadShader(std::string const & name, std::string const & filename = "");

		// List the names of all the shaders in the store.
		std::vector<std::string> listShaders() const;

		// Get a material from the store.
		UsePtr<Material> getMaterial(std::string const & name) const;

		// Load a material from a file.
		UsePtr<Material> loadMaterial(std::string const & name, std::string const & filename = "");

		// Create a blank material.
		UsePtr<Material> createMaterial(std::string const & name);

		// List the names of all the materials in the store.
		std::vector<std::string> listMaterials() const;

	private:
		Cache<ImageInternal> imageCache;
		Cache<Texture> textureCache;
		Cache<MeshInternal> meshCache;
		Cache<VertexBufferObject> vertexBufferObjectCache;
		Cache<Shader> shaderCache;
		Cache<Material> materialCache;
	};
}