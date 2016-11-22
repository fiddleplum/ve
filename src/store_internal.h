#pragma once

#include "store.h"
#include "render/image_internal.h"
#include "render/texture.h"
#include "render/mesh_internal.h"
#include "render/vertex_buffer_object.h"
#include "render/shader.h"
#include "render/material.h"
#include "util/font.h"
#include "util/cache.h"

namespace ve
{
	// Contains objects that reference outside sources such as the GPU or file system.
	class StoreInternal : public virtual Store
	{
	public:
		// Get an image from the store.
		Ptr<Image> getImage(std::string const & name) const override;

		// Get an image from the store.
		Ptr<ImageInternal> getImageInternal(std::string const & name) const;

		// Load an image into the store with the given name.
		Ptr<Image> loadImage(std::string const & name, std::string const & location = "") override;

		// Create a new image in the store with the given name.
		Ptr<Image> createImage(std::string const & name, Vector2i size, Image::Format format) override;

		// List the names of all the images in the store.
		std::vector<std::string> listImages() const override;

		// Get a texture from the store.
		Ptr<Texture> getTexture(std::string const & name) const;

		// Create a texture from an image in the store.
		Ptr<Texture> createTexture(std::string const & name, Ptr<Image> image);

		// Get a mesh from the store.
		Ptr<Mesh> getMesh(std::string const & name) const override;

		// Get a mesh from the store.
		Ptr<MeshInternal> getMeshInternal(std::string const & name) const;

		// Load a mesh into the store with the given name.
		Ptr<Mesh> loadMesh(std::string const & name, std::string const & location = "") override;

		// Create a new mesh in the store with the given name.
		Ptr<Mesh> createMesh(std::string const & name) override;

		// List the names of all the meshes in the store.
		std::vector<std::string> listMeshes() const override;

		// Get a vertex buffer object from the store.
		Ptr<VertexBufferObject> getVertexBufferObject(std::string const & name) const;

		// Create a vertex buffer object from an mesh in the store.
		Ptr<VertexBufferObject> createVertexBufferObject(std::string const & name, Ptr<Mesh> mesh = Ptr<Mesh>());

		// List the names of all the vertex buffer objects in the store.
		std::vector<std::string> listVertexBufferObjects() const;

		// Get a shader from the store.
		Ptr<Shader> getShader(std::string const & name) const;

		// Load a shader from a config.
		Ptr<Shader> loadShader(std::string const & name, Config const & config);

		// Load a shader from a file.
		Ptr<Shader> loadShader(std::string const & name, std::string const & filename = "");

		// List the names of all the shaders in the store.
		std::vector<std::string> listShaders() const;

		// Get a material from the store.
		Ptr<Material> getMaterial(std::string const & name) const;

		// Load a material from a file.
		Ptr<Material> loadMaterial(std::string const & name, std::string const & filename = "");

		// Create a blank material.
		Ptr<Material> createMaterial(std::string const & name);

		// List the names of all the materials in the store.
		std::vector<std::string> listMaterials() const;

		// Get a font from the store.
		Ptr<Font> getFont(std::string const & name) const;

		// Load a font from a file.
		Ptr<Font> loadFont(std::string const & name, std::string const & filename = "");

		// List the names of all the fonts in the store.
		std::vector<std::string> listFonts() const;

	private:
		Cache<ImageInternal> imageCache;
		Cache<Texture> textureCache;
		Cache<MeshInternal> meshCache;
		Cache<VertexBufferObject> vertexBufferObjectCache;
		Cache<Shader> shaderCache;
		Cache<Material> materialCache;
		Cache<Font> fontCache;
	};
}