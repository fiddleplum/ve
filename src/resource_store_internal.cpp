#include "resources_store_internal.h"

namespace ve
{
	UsePtr<Image> ResourceStoreInternal::getImage(std::string const & name) const
	{
		return imageCache.get(name);
	}

	UsePtr<ImageInternal> ResourceStoreInternal::getImageInternal(std::string const & name) const
	{
		return imageCache.get(name);
	}

	UsePtr<Image> ResourceStoreInternal::loadImage(std::string const & name, std::string const & location)
	{
		if (location != "")
		{
			return imageCache.create(name, location);
		}
		else
		{
			return imageCache.create(name, "assets/images/" + name + ".png");
		}
	}

	UsePtr<Image> ResourceStoreInternal::createImage(std::string const & name, Vector2i size, Image::Format format)
	{
		return imageCache.create(name, size, format);
	}

	std::vector<std::string> ResourceStoreInternal::listImages() const
	{
		return imageCache.getObjectNames();
	}

	UsePtr<Texture> ResourceStoreInternal::getTexture(std::string const & name) const
	{
		return textureCache.get(name);
	}

	UsePtr<Texture> ResourceStoreInternal::createTexture(std::string const & name, UsePtr<Image> image)
	{
		return textureCache.create(name, image);
	}

	UsePtr<Mesh> ResourceStoreInternal::getMesh(std::string const & name) const
	{
		return meshCache.get(name);
	}

	UsePtr<MeshInternal> ResourceStoreInternal::getMeshInternal(std::string const & name) const
	{
		return meshCache.get(name);
	}

	UsePtr<Mesh> ResourceStoreInternal::loadMesh(std::string const & name, std::string const & location)
	{
		if (location != "")
		{
			return meshCache.create(name, location);
		}
		else
		{
			return meshCache.create(name, "assets/meshes/" + name + ".mesh");
		}
	}

	UsePtr<Mesh> ResourceStoreInternal::createMesh(std::string const & name)
	{
		return meshCache.create(name);
	}

	std::vector<std::string> ResourceStoreInternal::listMeshes() const
	{
		return meshCache.getObjectNames();
	}

	UsePtr<VertexBufferObject> ResourceStoreInternal::getVertexBufferObject(std::string const & name) const
	{
		return vertexBufferObjectCache.get(name);
	}

	UsePtr<VertexBufferObject> ResourceStoreInternal::createVertexBufferObject(std::string const & name, UsePtr<Mesh> mesh)
	{
		if (mesh.isValid())
		{
			return vertexBufferObjectCache.create(name, mesh);
		}
		else
		{
			return vertexBufferObjectCache.create(name, loadMesh(name));
		}
	}

	std::vector<std::string> ResourceStoreInternal::listVertexBufferObjects() const
	{
		return vertexBufferObjectCache.getObjectNames();
	}

	UsePtr<Shader> ResourceStoreInternal::getShader(std::string const & name) const
	{
		return shaderCache.get(name);
	}

	UsePtr<Shader> ResourceStoreInternal::loadShader(std::string const & name, Config const & config)
	{
		return shaderCache.create(name, config);
	}

	UsePtr<Shader> ResourceStoreInternal::loadShader(std::string const & name, std::string const & filename)
	{
		if (filename != "")
		{
			return shaderCache.create(name, filename);
		}
		else
		{
			return shaderCache.create(name, "assets/shaders/" + name + ".shader");
		}
	}

	std::vector<std::string> ResourceStoreInternal::listShaders() const
	{
		return shaderCache.getObjectNames();
	}

	UsePtr<Material> ResourceStoreInternal::getMaterial(std::string const & name) const
	{
		return materialCache.get(name);
	}

	UsePtr<Material> ResourceStoreInternal::loadMaterial(std::string const & name, std::string const & filename)
	{
		if (filename != "")
		{
			return materialCache.create(name, filename);
		}
		else
		{
			return materialCache.create(name, "assets/materials/" + name + ".material");
		}
	}

	UsePtr<Material> ResourceStoreInternal::createMaterial(std::string const & name)
	{
		return materialCache.create(name);
	}

	std::vector<std::string> ResourceStoreInternal::listMaterials() const
	{
		return materialCache.getObjectNames();
	}
}