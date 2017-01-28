#pragma once

#include "render/vertex_buffer_object.hpp"
#include "render/shader.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "util/mesh.hpp"
#include "util/cache.hpp"

namespace ve
{
	// Contains objects that are typically used multiple times.
	class Store final
	{
	public:
		Cache<render::Image> images;
		Cache<Mesh> meshes;
		Cache<render::VertexBufferObject> vertexBufferObjects;
		Cache<render::Shader> shaders;
		Cache<render::Font> fonts;
	};

	Ptr<Store> getStore();
}

