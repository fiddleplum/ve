#pragma once

#include "util/ptr.h"
#include "render/scene.h"
#include "render/stage.h"
#include <set>

namespace ve
{
	class Renderer
	{
	public:
		// Constructor. Takes any SDL window to create a context.
		Renderer(void * sdlWindow);

		// Destructor.
		~Renderer();

		// Returns the gl context of the thread.
		void * getGlContext() const;

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