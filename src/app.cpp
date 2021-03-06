#include "ve.hpp"
#include "log.hpp"
#include "util/profiler.hpp"
#include <SDL.h>

namespace ve
{
	App::App(Config const & config)
	{
		// Initialize the profiler if it is configured.
		if (config.getChildAs<bool>("profile", false))
		{
			Profiler::initialize();
		}

		// Initialize the log.
		Log::initialize();

		// Initialize SDL.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}

		// Set the loop update time to the refresh rate.
		SDL_DisplayMode sdlDisplayMode;
		SDL_GetDisplayMode(0, 0, &sdlDisplayMode);
		secondsPerUpdate = 1.0f / sdlDisplayMode.refresh_rate;

		input.setNew();
	}

	App::~App()
	{
		windows.queueAllForErase();
		windows.processEraseQueue();
		worlds.queueAllForErase();
		worlds.processEraseQueue();

		input.setNull();

		SDL_Quit();

		Log::finalize();

		if (Profiler::isInitialized())
		{
			Profiler::finalize();
		}
	}

	void App::loop()
	{
		float lastFrameTime = (float)SDL_GetTicks() / 1000.0f;
		float accumulator = 0.f;
		looping = true;

		while (looping)
		{
			float currentFrameTime = (float)SDL_GetTicks() / 1000.f;

			// Handle events
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
				for (auto && window : windows)
				{
					window->update(secondsPerUpdate);
				}

				for (auto && world : worlds)
				{
					world->update(secondsPerUpdate);
				}

				if (updateCallback)
				{
					updateCallback(secondsPerUpdate);
				}

				accumulator -= secondsPerUpdate;
			}

			for (auto & window : windows)
			{
				window->preRender();
			}

			for (auto const & window : windows)
			{
				window->render();
			}

			// The loop might have temporal aliasing if the secondsPerUpdate is much less than the render frame rate.
			// This algorithm is from http://gafferongames.com/game-physics/fix-your-timestep.
			secondsPerLoop = currentFrameTime - lastFrameTime;
			accumulator += secondsPerLoop;
			lastFrameTime = currentFrameTime;

			// Do frame cleanup.
			windows.processEraseQueue();
		}

		if (quitCallback)
		{
			quitCallback();
		}
	}

	void App::quit()
	{
		looping = false;
	}

	float App::getSecondsPerUpdate() const
	{
		return secondsPerUpdate;
	}

	void App::setSecondsPerUpdate(float dt)
	{
		secondsPerUpdate = dt;
	}

	float App::getSecondsPerLoop() const
	{
		return secondsPerLoop;
	}

	Ptr<Window> App::createWindow()
	{
		auto window = windows.insertNew<Window>();
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
		return worlds.insertNew<world::World>();
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

	void App::setQuitCallback(std::function<void()> const & callback)
	{
		quitCallback = callback;
	}

	void App::setUpdateCallback(std::function<void(float secondsPerFrame)> const & callback)
	{
		updateCallback = callback;
	}

	void App::setInputEventCallback(std::function<void(InputEvent const & event)> const & callback)
	{
		inputEventCallback = callback;
	}

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
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_TEXTINPUT:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEWHEEL:
			{
				std::vector<InputEvent> inputEvents;
				input->populateFromSDLEvents(inputEvents, sdlEvent);
				for (auto && inputEvent : inputEvents)
				{
					if (window.isValid())
					{
						//window->handleInputEvent(inputEvent);
					}
					for (auto && world : worlds)
					{
						world->handleInputEvent(inputEvent);
					}
					if (inputEventCallback)
					{
						inputEventCallback(inputEvent);
					}
				}
				break;
			}
		}
	}
}

