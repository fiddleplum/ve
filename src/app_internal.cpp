#include "app_internal.h"
#include "ve.h"
#include "render/open_gl.h"
#include <SDL.h>

namespace ve
{
	AppInternal::AppInternal()
	{
		looping = true;
		secondsPerUpdate = 1.f / 24.f;
		resourceStore.setNew();

		// Start SDL.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}
	}

	void AppInternal::loop()
	{
		float lastFrameTime = SDL_GetTicks() / 1000.f;
		float accumulator = 0.f;
		while (looping)
		{
			float currentFrameTime = SDL_GetTicks() / 1000.f;

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
			while (accumulator >= secondsPerUpdate)
			{
				ve::update(secondsPerUpdate);

				for (auto & window : windows)
				{
					window->update(secondsPerUpdate);
				}
				//for (auto & scene : scenes)
				//{
				//	scene->update(dt);
				//}

				accumulator -= secondsPerUpdate;
			}

			// PreRender Update
			for (auto & window : windows)
			{
				//window->preRenderUpdate();
			}

			// Render (Scene render happens in each Viewport)
			for (auto const & window : windows)
			{
				window->render(renderer->getGlContext());
			}

			// The loop might have temporal aliasing if the targetSecondsPerFrame is much less than the render frame rate.
			// This algorithm is from http://gafferongames.com/game-physics/fix-your-timestep.
			accumulator += currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			// Do frame cleanup.
			windows.processEraseQueue();
			if (windows.empty())
			{
				renderer.setNull();
			}
		}
	}

	void AppInternal::quit()
	{
		looping = false;
	}

	Ptr<Window> AppInternal::createWindow()
	{
		auto window = OwnPtr<WindowInternal>::returnNew();
		if (windows.empty())
		{
			renderer.setNew(window->getSDLWindow());
		}
		windows.push_back(window);
		return window;
	}

	void AppInternal::destroyWindow(Ptr<Window> window)
	{
		auto it = std::find(windows.begin(), windows.end(), window);
		if (it == windows.end())
		{
			throw std::runtime_error("Window not found. ");
		}
		windows.queueForErase(it);
	}

	UsePtr<ResourceStore> AppInternal::getResourceStore() const
	{
		return resourceStore;
	}

	UsePtr<ResourceStoreInternal> AppInternal::getResourceStoreInternal() const
	{
		return resourceStore;
	}

	void AppInternal::handleSDLEvent(SDL_Event const & sdlEvent)
	{
		Ptr<WindowInternal> window;

		// Certain events are associated with a window. Get that window.
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
				break;
			case SDL_WINDOWEVENT:
				switch (sdlEvent.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						window->callCloseHandler();
						break;
					//case SDL_WINDOWEVENT_SIZE_CHANGED:
					//	window->handleResize({sdlEvent.window.data1, sdlEvent.window.data2});
					//	break;
					//case SDL_WINDOWEVENT_LEAVE:
					//	window->setCursorPosition(std::nullopt);
					//	break;
				}
			//case SDL_KEYDOWN:
			//case SDL_KEYUP:
			//{
			//	KeyboardEvent kevent;
			//	kevent.setFromSDL(sdlEvent.type, sdlEvent.key.keysym.sym);
			//	window->handleInputEvent(kevent);
			//	break;
			//}
			//case SDL_MOUSEBUTTONDOWN:
			//case SDL_MOUSEBUTTONUP:
			//{
			//	MouseButtonEvent mbevent;
			//	mbevent.setFromSDL(sdlEvent.type, sdlEvent.button.button);
			//	window->handleInputEvent(mbevent);
			//	break;
			//}
			//case SDL_MOUSEMOTION:
			//{
			//	MouseMoveEvent mmevent;
			//	mmevent.setFromSDL(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
			//	window->handleInputEvent(mmevent);
			//	break;
			//}
			//case SDL_MOUSEWHEEL:
			//{
			//	MouseWheelEvent mwevent;
			//	mwevent.setFromSDL(sdlEvent.wheel.y * (sdlEvent.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? 1 : -1));
			//	window->handleInputEvent(mwevent);
			//	break;
			//}
		}
	}

	Ptr<WindowInternal> AppInternal::getWindowFromId(unsigned int id)
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
		return Ptr<WindowInternal>();
	}
}