#include "renderer.h"
#include "open_gl.h"

namespace ve
{
	Renderer::Renderer(SDL_Window * sdlWindow)
	{
		glContext = SDL_GL_CreateContext(sdlWindow);
		glInitialize();
	}

	Renderer::~Renderer()
	{
		SDL_GL_DeleteContext(glContext);
		glContext = 0;
	}
}