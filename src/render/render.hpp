#pragma once

#include "render/shader.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "render/mesh.hpp"
#include "render/scene.hpp"
#include "render/target.hpp"
#include "util/cache.hpp"
#include "util/ptr_set.hpp"

/*I have decided to keep the render class. I don't like how other things create their own scene objects, so this will be put into here. No render:: object will
be created outside of the render class.

This ensures that all objects are cleaned up. I would also like to move the glcontext creation and desruction into here as well. This means WindowTarget creation needs to be moved in as well
so I can keep track of the number of windows.

I will also move all render:: creations from store into here for completeness.
*/

namespace ve
{
	namespace render
	{
		class Render final
		{
		public:
			Cache<render::Image> images;
			Cache<render::Mesh> meshes;
			Cache<render::Shader> shaders;
			Cache<render::Font> fonts;
			Cache<render::ImageTarget> imageTargets;
			Cache<render::WindowTarget> windowTargets;
			Cache<render::Scene> scenes;

			void preRenderUpdate();
		};
	}
}