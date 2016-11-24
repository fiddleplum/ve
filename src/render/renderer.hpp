#pragma once

#include "util/ptr.hpp"
#include "render/scene.hpp"
#include "render/stage.hpp"
#include <set>

namespace ve
{
	class Renderer
	{
	public:
		// Constructor. Takes any SDL window to create a context.
		Renderer();

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
		std::set<OwnPtr<Scene>, std::less<>> scenes;
		std::set<OwnPtr<TextureStage>, std::less<>> textureStages;
	};
}