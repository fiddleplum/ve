#pragma once

#include <SDL.h>

namespace ve
{
	class Renderer
	{
	public:
		Renderer(SDL_Window * sdlWindow);

		~Renderer();

		void render();

	private:
		SDL_GLContext glContext;
	};
}