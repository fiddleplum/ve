#include "renderer.h"
#include "open_gl.h"

namespace ve
{
	Renderer::Renderer(SDL_Window * sdlWindow)
	{
		glContext = SDL_GL_CreateContext(sdlWindow);
		int result = SDL_GL_MakeCurrent(sdlWindow, glContext);
		glInitialize();
		char *version = (char*)glGetString(GL_VERSION);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		GLuint gProgramID = glCreateProgram();
		GLuint buffer;
		glGenBuffers(1, &buffer);
	}

	Renderer::~Renderer()
	{
		SDL_GL_DeleteContext(glContext);
		glContext = 0;
	}

	SDL_GLContext Renderer::getGlContext() const
	{
		return glContext;
	}

	UsePtr<Scene> Renderer::createScene()
	{
		auto scene = OwnPtr<Scene>::returnNew();
		scenes.insert(scene);
		return scene;
	}

	void Renderer::destroyScene(UsePtr<Scene> scene)
	{
		auto it = scenes.find(scene);
		if (it == scenes.end())
		{
			throw std::runtime_error("Scene not found while erasing. ");
		}
		scenes.erase(it);
	}

	UsePtr<TextureStage> Renderer::createTextureStage()
	{
		auto stage = OwnPtr<TextureStage>::returnNew();
		textureStages.insert(stage);
		return stage;
	}

	void Renderer::destroyTextureStage(UsePtr<TextureStage> stage)
	{
		auto it = textureStages.find(stage);
		if (it == textureStages.end())
		{
			throw std::runtime_error("Stage not found while erasing. ");
		}
		textureStages.erase(it);
	}
}