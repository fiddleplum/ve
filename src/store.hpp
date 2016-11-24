#pragma once

#include "render/image.h"
#include "render/texture.h"
#include "render/mesh.h"
#include "render/vertex_buffer_object.h"
#include "render/shader.h"
#include "render/material.h"
#include "render/font.h"
#include "util/cache.h"

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
		Cache<Material> materials;
		Cache<Font> fonts;
	};

	extern Store store;
}