#pragma once

#include "ptr.h"
#include "scene.h"
#include <SDL.h>
#include <set>

namespace ve
{
	class Stage;

	class Renderer
	{
	public:
		// Constructor. Takes any SDL window to create a context.
		Renderer(SDL_Window * sdlWindow);

		// Destructor.
		~Renderer();

		// Create a new scene.
		UsePtr<Scene> createScene();

		// Destroy a scene.
		void destroyScene(UsePtr<Scene> scene);

		// Render a given stage.
		void render(UsePtr<Stage> stage);

	private:
		SDL_GLContext glContext;
		std::set<OwnPtr<Scene>, std::less<>> scenes;
	};
}