#pragma once

#include "util/ptr.hpp"
#include "render/scene.hpp"
#include "render/stage.hpp"
#include <set>

namespace ve
{
	namespace render
	{
		class Renderer
		{
		public:
			// Constructor.
			Renderer(void * sdlWindow);

			// Destructor.
			~Renderer();

			// Create a new scene.
			Ptr<Scene> createScene();

			// Destroy a scene.
			void destroyScene(Ptr<Scene> scene);

			// Creates a new stage.
			Ptr<TextureStage> createTextureStage();

			// Destroys a stage.
			void destroyTextureStage(Ptr<TextureStage> stage);

		private:
			void * glContext;
			std::set<OwnPtr<Scene>, std::less<>> scenes;
			std::set<OwnPtr<TextureStage>, std::less<>> textureStages;
		};
	}
}