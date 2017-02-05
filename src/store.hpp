#pragma once

#include "render/shader.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "render/mesh.hpp"
#include "util/cache.hpp"

namespace ve
{
	// Contains objects that are typically used multiple times.
	class Store final
	{
	public:
		Cache<render::Image> images;
		Cache<render::Mesh> meshes;
		Cache<render::Shader> shaders;
		Cache<render::Font> fonts;
	};
}

