#pragma once

#include "ptr.h"
#include <SDL.h>

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

		// Render a given stage.
		void render(UsePtr<Stage> stage);

	private:
		SDL_GLContext glContext;
	};
}