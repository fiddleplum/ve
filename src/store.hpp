#pragma once

#include "render/texture.hpp"
#include "render/vertex_buffer_object.hpp"
#include "render/shader.hpp"
#include "render/font.hpp"
#include "util/mesh.hpp"
#include "util/image.hpp"
#include "util/cache.hpp"

namespace ve
{
	// Contains objects that are typically used multiple times.
	class Store final
	{
	public:
		Cache<Image> images;
		Cache<Mesh> meshes;
		Cache<render::Texture> textures;
		Cache<render::VertexBufferObject> vertexBufferObjects;
		Cache<render::Shader> shaders;
		Cache<render::Font> fonts;
	};

	Ptr<Store> getStore();
}

