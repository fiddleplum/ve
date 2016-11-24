#include "store.h"
#include <fstream>

namespace ve
{
	Ptr<Image> Store::getImage(std::string const & name) const
	{
		return imageCache.get(name);
	}

	Ptr<Image> Store::loadImage(std::string const & name, std::string const & location)
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

	Ptr<Image> Store::createImage(std::string const & name, Vector2i size, Image::Format format)
	{
		return imageCache.create(name, size, format);
	}

	Ptr<Image> Store::createImage(std::string const & name, void const * surface)
	{
		return imageCache.create(name, surface);
	}

	std::vector<std::string> Store::listImages() const
	{
		return imageCache.getObjectNames();
	}

	Ptr<Texture> Store::getTexture(std::string const & name) const
	{
		return textureCache.get(name);
	}

	Ptr<Texture> Store::createTexture(std::string const & name, Ptr<Image> image)
	{
		return textureCache.create(name, image);
	}

	Ptr<Mesh> Store::getMesh(std::string const & name) const
	{
		return meshCache.get(name);
	}

	Ptr<Mesh> Store::loadMesh(std::string const & name, std::string const & location)
	{
		std::ifstream in;
		if (location != "")
		{
			in.open(location, std::ios::in | std::ios::binary);
		}
		else
		{
			in.open("assets/meshes/" + name + ".mesh", std::ios::in | std::ios::binary);
		}
		return meshCache.create(name, in);
	}

	Ptr<Mesh> Store::createMesh(std::string const & name)
	{
		return meshCache.create(name);
	}

	std::vector<std::string> Store::listMeshes() const
	{
		return meshCache.getObjectNames();
	}

	Ptr<VertexBufferObject> Store::getVertexBufferObject(std::string const & name) const
	{
		return vertexBufferObjectCache.get(name);
	}

	Ptr<VertexBufferObject> Store::createVertexBufferObject(std::string const & name, Ptr<Mesh> mesh)
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

	std::vector<std::string> Store::listVertexBufferObjects() const
	{
		return vertexBufferObjectCache.getObjectNames();
	}

	Ptr<Shader> Store::getShader(std::string const & name) const
	{
		return shaderCache.get(name);
	}

	Ptr<Shader> Store::loadShader(std::string const & name, Config const & config)
	{
		return shaderCache.create(name, config);
	}

	Ptr<Shader> Store::loadShader(std::string const & name, std::string const & filename)
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

	std::vector<std::string> Store::listShaders() const
	{
		return shaderCache.getObjectNames();
	}

	Ptr<Material> Store::getMaterial(std::string const & name) const
	{
		return materialCache.get(name);
	}

	Ptr<Material> Store::loadMaterial(std::string const & name, std::string const & filename)
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

	Ptr<Material> Store::createMaterial(std::string const & name)
	{
		return materialCache.create(name);
	}

	std::vector<std::string> Store::listMaterials() const
	{
		return materialCache.getObjectNames();
	}

	Ptr<Font> Store::getFont(std::string const & name) const
	{
		return fontCache.get(name);
	}

	Ptr<Font> Store::loadFont(std::string const & name, std::string const & fontFace, int size)
	{
		return fontCache.create(name, "assets/fonts/" + fontFace + ".ttf", size);
	}

	std::vector<std::string> Store::listFonts() const
	{
		return fontCache.getObjectNames();
	}
}