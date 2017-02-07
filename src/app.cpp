#include "store.hpp"
#include "ve.hpp"
#include <SDL.h>

namespace ve
{
	App::App(std::vector<std::string> const & args)
	{
		// Initialize SDL.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}

		store.setNew();
	}

	App::~App()
	{
		windows.queueAllForErase();
		windows.processEraseQueue();
		worlds.queueAllForErase();
		worlds.processEraseQueue();

		store.setNull();

		SDL_Quit();
	}

	void App::loop()
	{
		float lastFrameTime = SDL_GetTicks() / 1000.f;
		float accumulator = 0.f;
		looping = true;
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
				if (updateCallback)
				{
					updateCallback(secondsPerUpdate);
				}

				for (auto & window : windows)
				{
					window->update(secondsPerUpdate);
				}

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
				window->render();
			}

			// The loop might have temporal aliasing if the targetSecondsPerFrame is much less than the render frame rate.
			// This algorithm is from http://gafferongames.com/game-physics/fix-your-timestep.
			accumulator += currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			// Do frame cleanup.
			windows.processEraseQueue();
		}
	}

	void App::quit()
	{
		looping = false;
	}

	Ptr<Window> App::createWindow()
	{
		auto window = *windows.insertNew();
		window->setCloseRequestedHandler(std::bind(&App::quit, this));
		return window;
	}

	void App::destroyWindow(Ptr<Window> const & window)
	{
		windows.queueForErase(window);
		if (!looping) // not looping so we're safe to just erase it.
		{
			windows.processEraseQueue();
		}
	}

	Ptr<world::World> App::createWorld()
	{
		return *worlds.insertNew();
	}

	void App::destroyWorld(Ptr<world::World> const & world)
	{
		worlds.queueForErase(world);
	}

	void App::showMessage(std::string const & message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", message.c_str(), nullptr);
	}

	Ptr<Store> App::getStore()
	{
		return store;
	}

	void App::setUpateCallback(std::function<void(float dt)> const & callback)
	{
		updateCallback = callback;
	}

	//void App::setInputEventCallback(std::function<void(InputEvent const & event)> const & callback)
	//{
	//	inputEventCallback = callback;
	//}

	void App::setRequestQuitCallback(std::function<void()> const & callback)
	{
		requestQuitCallback = callback;
	}

	// SDL has its own window IDs for SDL_Events. This gets the right window associated with that ID. Returns null if none found.
	Ptr<Window> App::getWindowFromId(unsigned int id)
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
		return Ptr<Window>();
	}

	// This handles a single SDL_Event. Lots of things happen here.
	void App::handleSDLEvent(SDL_Event const & sdlEvent)
	{
		Ptr<Window> window;

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

		// Handle the events.
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
				if (requestQuitCallback)
				{
					requestQuitCallback();
				}
				break;
			case SDL_WINDOWEVENT:
				switch (sdlEvent.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						window->onCloseRequested();
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						window->onResized({sdlEvent.window.data1, sdlEvent.window.data2});
						break;
					case SDL_WINDOWEVENT_LEAVE:
						window->onCursorPositionChanged(std::nullopt);
						break;
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
}

