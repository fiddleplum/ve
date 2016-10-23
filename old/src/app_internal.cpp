#include "app.h"
//#include "render/open_gl.h"
//#include "window_internal.h"
//#include "input_system.h"
//#include "resources.h"
//#include "scene.h"

//#include "audio.h"

namespace ve
{
	namespace internal
	{
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
				for (auto & scene : scenes)
				{
					scene->update(dt);
				}

				// PreRender Update
				for (auto & window : windows)
				{
					window->preRenderUpdate();
				}

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

		void quit()
		{
			looping = false;
		}

		UsePtr<Window> createWindow(std::string const & title)
		{
			auto window = OwnPtr<WindowInternal>::createNew(title);
			if (windows.empty())
			{
				glContext = SDL_GL_CreateContext(window->getSDLWindow());
				glInitialize();
			}
			windows.insert(window);
			return window;
		}

		void destroyWindow(UsePtr<Window> window)
		{
			if (!window.isValid())
			{
				throw std::runtime_error("Invalid window.");
			}
			if (!scenes.empty() && windows.size() == 1)
			{
				throw std::runtime_error("All scenes must be removed before the last window is removed, because the OpenGL context is destroyed. ");
			}
			windows.erase(std::find(windows.begin(), windows.end(), window));
			if (windows.empty())
			{
				SDL_GL_DeleteContext(glContext);
				glContext = 0;
			}
		}

		UsePtr<scene::Scene> createScene()
		{
			if (windows.empty())
			{
				throw std::runtime_error("A scene may not be created until a window has been created, because it needs an OpenGL context. ");
			}
			auto scene = OwnPtr<scene::Scene>::createNew();
			scenes.insert(scene);
			return scene;
		}

		void destroyScene(UsePtr<scene::Scene> scene)
		{
			if (!scene.isValid())
			{
				throw std::runtime_error("Invalid scene.");
			}
			scenes.erase(std::find(scenes.begin(), scenes.end(), scene));
		}

		void showMessage(std::string const & message)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", message.c_str(), nullptr);
		}

		void handleSDLEvent(SDL_Event const & sdlEvent)
		{
			UsePtr<Window> window;
			switch (sdlEvent.type)
			{
				case SDL_WINDOWEVENT:
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEMOTION:
				case SDL_MOUSEWHEEL:
					window = getWindowFromId(sdlEvent.key.windowID);
					if (!window.isValid())
					{
						return;
					}
			}
			switch (sdlEvent.type)
			{
				case SDL_QUIT:
					quit();
					break;
				case SDL_WINDOWEVENT:
					switch (sdlEvent.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
							quit();
							break;
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							window->handleResize({sdlEvent.window.data1, sdlEvent.window.data2});
							break;
						case SDL_WINDOWEVENT_LEAVE:
							window->setCursorPosition(std::nullopt);
							break;
					}
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					KeyboardEvent kevent;
					kevent.setFromSDL(sdlEvent.type, sdlEvent.key.keysym.sym);
					window->handleInputEvent(kevent);
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				{
					MouseButtonEvent mbevent;
					mbevent.setFromSDL(sdlEvent.type, sdlEvent.button.button);
					window->handleInputEvent(mbevent);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					MouseMoveEvent mmevent;
					mmevent.setFromSDL(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
					window->handleInputEvent(mmevent);
					break;
				}
				case SDL_MOUSEWHEEL:
				{
					MouseWheelEvent mwevent;
					mwevent.setFromSDL(sdlEvent.wheel.y * (sdlEvent.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? 1 : -1));
					window->handleInputEvent(mwevent);
					break;
				}
			}
		}

		UsePtr<Window> getWindowFromId(unsigned int id)
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
			return UsePtr<Window>();
		}

		//void setCursorActive(bool)
		//{
		//	// TODO
		//}
	}
}

#include "app.h"
#include <SDL.h>

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
#undef main
	std::vector<std::string> args;
	try
	{
		// Grab the params.
		for (int i = 1; i < argc; ++i) // don't include the 0th arg, because it is the program name
		{
			args.push_back(std::string(argv[i]));
		}

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

		ve::main(args);

		//scenes.clear();
		ve::windows.clear();
		// Destroy the singletons.
		//fontCache.setNull();
		//textureCache.setNull();
		//shaderCache.setNull();
		//SceneModelCache::destroyInstance();
		//InputSystem::destroyInstance();

		// Stop SDL.
		SDL_Quit();
	}
	catch (std::exception const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}
	return 0;
}

