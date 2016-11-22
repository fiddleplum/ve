#include "store_internal.h"

namespace ve
{
	Ptr<Image> StoreInternal::getImage(std::string const & name) const
	{
		return imageCache.get(name);
	}

	Ptr<ImageInternal> StoreInternal::getImageInternal(std::string const & name) const
	{
		return imageCache.get(name);
	}

	Ptr<Image> StoreInternal::loadImage(std::string const & name, std::string const & location)
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

	Ptr<Image> StoreInternal::createImage(std::string const & name, Vector2i size, Image::Format format)
	{
		return imageCache.create(name, size, format);
	}

	std::vector<std::string> StoreInternal::listImages() const
	{
		return imageCache.getObjectNames();
	}

	Ptr<Texture> StoreInternal::getTexture(std::string const & name) const
	{
		return textureCache.get(name);
	}

	Ptr<Texture> StoreInternal::createTexture(std::string const & name, Ptr<Image> image)
	{
		return textureCache.create(name, image);
	}

	Ptr<Mesh> StoreInternal::getMesh(std::string const & name) const
	{
		return meshCache.get(name);
	}

	Ptr<MeshInternal> StoreInternal::getMeshInternal(std::string const & name) const
	{
		return meshCache.get(name);
	}

	Ptr<Mesh> StoreInternal::loadMesh(std::string const & name, std::string const & location)
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

	Ptr<Mesh> StoreInternal::createMesh(std::string const & name)
	{
		return meshCache.create(name);
	}

	std::vector<std::string> StoreInternal::listMeshes() const
	{
		return meshCache.getObjectNames();
	}

	Ptr<VertexBufferObject> StoreInternal::getVertexBufferObject(std::string const & name) const
	{
		return vertexBufferObjectCache.get(name);
	}

	Ptr<VertexBufferObject> StoreInternal::createVertexBufferObject(std::string const & name, Ptr<Mesh> mesh)
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

	std::vector<std::string> StoreInternal::listVertexBufferObjects() const
	{
		return vertexBufferObjectCache.getObjectNames();
	}

	Ptr<Shader> StoreInternal::getShader(std::string const & name) const
	{
		return shaderCache.get(name);
	}

	Ptr<Shader> StoreInternal::loadShader(std::string const & name, Config const & config)
	{
		return shaderCache.create(name, config);
	}

	Ptr<Shader> StoreInternal::loadShader(std::string const & name, std::string const & filename)
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

	std::vector<std::string> StoreInternal::listShaders() const
	{
		return shaderCache.getObjectNames();
	}

	Ptr<Material> StoreInternal::getMaterial(std::string const & name) const
	{
		return materialCache.get(name);
	}

	Ptr<Material> StoreInternal::loadMaterial(std::string const & name, std::string const & filename)
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

	Ptr<Material> StoreInternal::createMaterial(std::string const & name)
	{
		return materialCache.create(name);
	}

	std::vector<std::string> StoreInternal::listMaterials() const
	{
		return materialCache.getObjectNames();
	}

	Ptr<Font> StoreInternal::getFont(std::string const & name) const
	{
		return fontCache.get(name);
	}

	Ptr<Font> StoreInternal::loadFont(std::string const & name, std::string const & filename)
	{
		if (filename != "")
		{
			return fontCache.create(name, filename);
		}
		else
		{
			return fontCache.create(name, "assets/fonts/" + name + ".ttf");
		}
	}

	std::vector<std::string> StoreInternal::listFonts() const
	{
		return fontCache.getObjectNames();
	}
}