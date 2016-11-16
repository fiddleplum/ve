#pragma once

#include "util/ptr.h"
#include "render/scene.h"
#include "render/stage.h"
#include <SDL.h>
#include <set>

namespace ve
{
	class Renderer
	{
	public:
		// Constructor. Takes any SDL window to create a context.
		Renderer(SDL_Window * sdlWindow);

		// Destructor.
		~Renderer();

		// Returns the gl context of the thread.
		SDL_GLContext getGlContext() const;

		// Create a new scene.
		UsePtr<Scene> createScene();

		// Destroy a scene.
		void destroyScene(UsePtr<Scene> scene);

		// Creates a new stage.
		UsePtr<TextureStage> createTextureStage();

		// Destroys a stage.
		void destroyTextureStage(UsePtr<TextureStage> stage);

	private:
		SDL_GLContext glContext;
		std::set<OwnPtr<Scene>, std::less<>> scenes;
		std::set<OwnPtr<TextureStage>, std::less<>> textureStages;
	};
}