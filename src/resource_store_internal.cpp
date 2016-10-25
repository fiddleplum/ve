#include "resources_store_internal.h"

namespace ve
{
	UsePtr<Image> ResourceStoreInternal::getImage(std::string const & name)
	{
		return imageCache.get(name);
	}

	UsePtr<ImageInternal> ResourceStoreInternal::getImageInternal(std::string const & name)
	{
		return imageCache.get(name);
	}

	void ResourceStoreInternal::loadImage(std::string const & name, std::string const & location)
	{
		imageCache.create(name, location);
	}

	void ResourceStoreInternal::createImage(std::string const & name, Coord2i size, Image::Format format)
	{
		imageCache.create(name, size, format);
	}

	std::vector<std::string> ResourceStoreInternal::listImages() const
	{
		return imageCache.getObjectNames();
	}

	UsePtr<Texture> ResourceStoreInternal::getTexture(std::string const & name)
	{
		return textureCache.get(name);
	}

	void ResourceStoreInternal::createTexture(std::string const & name, UsePtr<Image> image)
	{
		textureCache.create(name, image);
	}

	UsePtr<Mesh> ResourceStoreInternal::getMesh(std::string const & name)
	{
		return meshCache.get(name);
	}

	UsePtr<MeshInternal> ResourceStoreInternal::getMeshInternal(std::string const & name)
	{
		return meshCache.get(name);
	}

	void ResourceStoreInternal::loadMesh(std::string const & name, std::string const & location)
	{
		meshCache.create(name, location);
	}

	void ResourceStoreInternal::createMesh(std::string const & name)
	{
		meshCache.create(name);
	}

	std::vector<std::string> ResourceStoreInternal::listMeshes() const
	{
		return meshCache.getObjectNames();
	}

	UsePtr<VertexBufferObject> ResourceStoreInternal::getVertexBufferObject(std::string const & name)
	{
		return vertexBufferObjectCache.get(name);
	}

	void ResourceStoreInternal::creaateVertexBufferObject(std::string const & name, UsePtr<Mesh> mesh)
	{
		vertexBufferObjectCache.create(name, mesh);
	}
}