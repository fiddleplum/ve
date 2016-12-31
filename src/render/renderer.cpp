#include "renderer.hpp"
#include "open_gl.hpp"
#include <SDL.h>

namespace ve
{
	namespace render
	{
		Renderer::Renderer(void * sdlWindow)
		{
			glContext = SDL_GL_CreateContext((SDL_Window *)sdlWindow);
			SDL_GL_MakeCurrent((SDL_Window *)sdlWindow, glContext);
			glInitialize();

			char *version = (char*)glGetString(GL_VERSION);

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		}

		Renderer::~Renderer()
		{
			SDL_GL_DeleteContext(glContext);
		}

		Ptr<Scene> Renderer::createScene()
		{
			auto scene = OwnPtr<Scene>::returnNew();
			scenes.insert(scene);
			return scene;
		}

		void Renderer::destroyScene(Ptr<Scene> scene)
		{
			auto it = scenes.find(scene);
			if (it == scenes.end())
			{
				throw std::runtime_error("Scene not found while erasing. ");
			}
			scenes.erase(it);
		}

		Ptr<TextureStage> Renderer::createTextureStage()
		{
			auto stage = OwnPtr<TextureStage>::returnNew();
			textureStages.insert(stage);
			return stage;
		}

		void Renderer::destroyTextureStage(Ptr<TextureStage> stage)
		{
			auto it = textureStages.find(stage);
			if (it == textureStages.end())
			{
				throw std::runtime_error("Stage not found while erasing. ");
			}
			textureStages.erase(it);
		}
	}
}