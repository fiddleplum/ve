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

		input.setNew();
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
				update(secondsPerUpdate);

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
		return *worlds.insertNew<world::World>();
	}

	void App::destroyWorld(Ptr<world::World> const & world)
	{
		worlds.queueForErase(world);
	}

	void App::showMessage(std::string const & message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", message.c_str(), nullptr);
	}

	Ptr<Input> App::getInput() const
	{
		return input;
	}

	Ptr<Store> App::getStore() const
	{
		return store;
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
			case SDL_TEXTINPUT:
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
				requestQuit();
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
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_TEXTINPUT:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEWHEEL:
			{
				std::vector<Input::Event> inputEvents;
				input->populateFromSDLEvents(inputEvents, sdlEvent);
				for (auto && inputEvent : inputEvents)
				{
					if (window.isValid())
					{
						//window->handleInputEven(inputEvent);
					}
					handleInputEvent(inputEvent);
				}
				break;
			}
		}
	}
}

