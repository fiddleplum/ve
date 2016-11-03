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

	UsePtr<Scene> Renderer::createScene()
	{
		auto scene = OwnPtr<Scene>::createNew();
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
		auto stage = OwnPtr<TextureStage>::createNew();
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