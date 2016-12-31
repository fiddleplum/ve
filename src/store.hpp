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
		Cache<Texture> textures;
		Cache<Mesh> meshes;
		Cache<VertexBufferObject> vertexBufferObjects;
		Cache<Shader> shaders;
		Cache<Font> fonts;
	};

	extern Store store;
}