#include "app.h"
#include "open_gl.h"
//#include "input_system.h"
//#include "resources.h"
//#include "window.h"
//#include "scene.h"
#include <SDL.h>

//#include "audio.h"

namespace ve
{
	OwnPtr<App> app;

	App::App()
	{
		glContext = nullptr;
		looping = false;
		targetFrameRate = 60.f;

		// Start SDL.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}

		// Initialize the singletons.
		//InputSystem::createInstance();
		//shaderCache.setNew();
		//textureCache.setNew();
		//fontCache.setNew();
		//SceneModelCache::createInstance();

		//SDL_InitSubSystem(SDL_INIT_AUDIO);
	}

	App::~App()
	{
		//scenes.clear();
		windows.clear();
		// Destroy the singletons.
		//fontCache.setNull();
		//textureCache.setNull();
		//shaderCache.setNull();
		//SceneModelCache::destroyInstance();
		//InputSystem::destroyInstance();

		// Stop SDL.
		SDL_Quit();
	}

	void App::quit()
	{
		looping = false;
	}

	UsePtr<gui::Window> App::addWindow(std::string const & title)
	{
		OwnPtr<gui::Window> window;
		window.setNew(title);
		if (windows.empty())
		{
			glContext = SDL_GL_CreateContext(window->getSDLWindow());
			glInitialize();
		}
		windows.insert(window);
		return window;
	}

	void App::removeWindow(UsePtr<gui::Window> window)
	{
		if (!window.isValid())
		{
			throw std::runtime_error("Invalid window.");
		}
		//if (!scenes.empty() && windows.size() == 1)
		//{
		//	throw std::runtime_error("All scenes must be removed before the last window is removed.");
		//}
		windows.erase(window);
		if (windows.empty())
		{
			SDL_GL_DeleteContext(glContext);
			glContext = 0;
		}
	}

	//Ptr<Scene> App::addScene()
	//{
	//	if (windows.empty())
	//	{
	//		throw std::runtime_error("A scene may not be created until a window has been created.");
	//	}
	//	OwnPtr<Scene> scene;
	//	scene.setNew();
	//	scenes.insert(scene);
	//	return scene;
	//}

	//void App::removeScene(Ptr<Scene> scene)
	//{
	//	if (!scene.isValid())
	//	{
	//		throw std::runtime_error("Invalid scene.");
	//	}
	//	scenes.erase(scene);
	//}

	void App::showMessage(std::string const & message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", message.c_str(), nullptr);
	}

	void App::loop()
	{
		looping = true;
		while (looping)
		{
			float frameStartTime = SDL_GetTicks() / 1000.f;

			//// Handle events
			//controllers::startFrame();
			SDL_Event sdlEvent;
			while (SDL_PollEvent(&sdlEvent))
			{
				handleSDLEvent(sdlEvent);
			}
			//for(int i = 0; i < controllers::getNumControllers(); i++)
			//{
			//	std::vector<std::pair<int, float>> controllerAxisEvents = controllers::getAxesChangedSinceLastFrame(i);
			//	for(auto controllerAxisEvent : controllerAxisEvents)
			//	{
			//		ControllerAxisEvent event{Ptr<Window>()};
			//		event.controller = i;
			//		event.axis = controllerAxisEvent.first;
			//		event.value = controllerAxisEvent.second;
			//		handleEvent(event);
			//	}
			//}

			// Update
			float dt = 1.f / targetFrameRate;
			for (auto & window : windows)
			{
				window->update(dt);
			}
			//for (auto & scene : scenes)
			//{
			//	scene->update(dt);
			//}

			// PreRender Update
			for (auto & window : windows)
			{
				window->preRenderUpdate();
			}
			//for (auto scene : scenes)
			//{
			//	scene->preRenderUpdate();
			//}

			// Render (Scene render happens in each Viewport)
			for (auto const & window : windows)
			{
				window->render(glContext);
			}

			// FIX THIS: Introduce better loop timing
			float delayTime = (1.f / targetFrameRate) - (SDL_GetTicks() / 1000.f - frameStartTime);
			if (delayTime > 0)
			{
				SDL_Delay((unsigned int)(delayTime * 1000));
			}
		}
	}

	void App::handleSDLEvent(SDL_Event const & sdlEvent)
	{
		UsePtr<gui::Window> window;
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			quit();
			break;
		case SDL_WINDOWEVENT:
			window = getWindowFromId(sdlEvent.window.windowID);
			if (!window.isValid())
			{
				return;
			}
			switch (sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				quit();
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window->handleResize({ sdlEvent.window.data1, sdlEvent.window.data2 });
				break;
			case SDL_WINDOWEVENT_LEAVE:
				window->setCursorPosition(std::nullopt);
				break;
			}
			break;
		}
	}

	UsePtr<gui::Window> App::getWindowFromId(unsigned int id) const
	{
		SDL_Window * sdlWindow = SDL_GetWindowFromID(id);
		if (sdlWindow != NULL)
		{
			for (auto const & window : windows)
			{
				if (sdlWindow == window->getSDLWindow())
				{
					return window;
				}
			}
		}
		return UsePtr<gui::Window>();
	}

	//void App::_setCursorPosition(int windowId, Coord2i position)
	//{
	//	Ptr<Window> window = getWindowFromId(windowId);
	//	if(window.isValid())
	//	{
	//		window->setCursorPosition(position);
	//	}
	//}
	//
	//void setCursorActive(bool)
	//{
	//	// TODO
	//}

	//void App::handleEvent(Event const & event)
	//{
	//	for(auto window : app->windows)
	//	{
	//		window->handleEvent(event);
	//	}
	//	for(auto scene : app->scenes)
	//	{
	//		scene->handleEvent(event);
	//	}
	//}
	//
}

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
	std::vector<std::string> args;
	try
	{
		// Grab the params.
		for(int i = 1; i < argc; ++i) // don't include the 0th arg, because it is the program name
		{
			args.push_back(std::string(argv[i]));
		}
		ve::app.setNew();
		ve::onAppOpen(args);
		ve::app->loop();
		ve::onAppClose();
		ve::app.setNull();
	}
	catch(std::exception const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}
	return 0;
}

