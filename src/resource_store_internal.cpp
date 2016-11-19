#include "resources_store_internal.h"

namespace ve
{
	Ptr<Image> ResourceStoreInternal::getImage(std::string const & name) const
	{
		return imageCache.get(name);
	}

	Ptr<ImageInternal> ResourceStoreInternal::getImageInternal(std::string const & name) const
	{
		return imageCache.get(name);
	}

	Ptr<Image> ResourceStoreInternal::loadImage(std::string const & name, std::string const & location)
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

	Ptr<Image> ResourceStoreInternal::createImage(std::string const & name, Vector2i size, Image::Format format)
	{
		return imageCache.create(name, size, format);
	}

	std::vector<std::string> ResourceStoreInternal::listImages() const
	{
		return imageCache.getObjectNames();
	}

	Ptr<Texture> ResourceStoreInternal::getTexture(std::string const & name) const
	{
		return textureCache.get(name);
	}

	Ptr<Texture> ResourceStoreInternal::createTexture(std::string const & name, Ptr<Image> image)
	{
		return textureCache.create(name, image);
	}

	Ptr<Mesh> ResourceStoreInternal::getMesh(std::string const & name) const
	{
		return meshCache.get(name);
	}

	Ptr<MeshInternal> ResourceStoreInternal::getMeshInternal(std::string const & name) const
	{
		return meshCache.get(name);
	}

	Ptr<Mesh> ResourceStoreInternal::loadMesh(std::string const & name, std::string const & location)
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

	Ptr<Mesh> ResourceStoreInternal::createMesh(std::string const & name)
	{
		return meshCache.create(name);
	}

	std::vector<std::string> ResourceStoreInternal::listMeshes() const
	{
		return meshCache.getObjectNames();
	}

	Ptr<VertexBufferObject> ResourceStoreInternal::getVertexBufferObject(std::string const & name) const
	{
		return vertexBufferObjectCache.get(name);
	}

	Ptr<VertexBufferObject> ResourceStoreInternal::createVertexBufferObject(std::string const & name, Ptr<Mesh> mesh)
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

	Ptr<Shader> ResourceStoreInternal::getShader(std::string const & name) const
	{
		return shaderCache.get(name);
	}

	Ptr<Shader> ResourceStoreInternal::loadShader(std::string const & name, Config const & config)
	{
		return shaderCache.create(name, config);
	}

	Ptr<Shader> ResourceStoreInternal::loadShader(std::string const & name, std::string const & filename)
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

	Ptr<Material> ResourceStoreInternal::getMaterial(std::string const & name) const
	{
		return materialCache.get(name);
	}

	Ptr<Material> ResourceStoreInternal::loadMaterial(std::string const & name, std::string const & filename)
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

	Ptr<Material> ResourceStoreInternal::createMaterial(std::string const & name)
	{
		return materialCache.create(name);
	}

	std::vector<std::string> ResourceStoreInternal::listMaterials() const
	{
		return materialCache.getObjectNames();
	}
}