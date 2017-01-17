#include "util/ptr_list.hpp"
#include "util/ptr_set.hpp"
#include "store.hpp"
#include "ve.hpp"
#include <SDL.h>

namespace ve
{
	bool looping = false;
	float secondsPerUpdate = 1.f / 24.f;
	std::function<void(float dt)> updateFunction;
	PtrSet<Window> windows;
	PtrSet<world::World> worlds;
	Store store;

	// SDL has its own window IDs for SDL_Events. This gets the right window associated with that ID. Returns null if none found.
	Ptr<Window> getWindowFromId(unsigned int id)
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
	void handleSDLEvent(SDL_Event const & sdlEvent)
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
				userRequestQuit();
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

	// This is the main application loop. It keeps going until looping is false.
	void loop()
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
				userUpdate(secondsPerUpdate);

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

	void quit()
	{
		looping = false;
	}

	void setUpdateFunction(std::function<void(float dt)> const & function)
	{

	}

	Ptr<Window> createWindow()
	{
		auto window = *windows.insertNew();
		window->setCloseRequestedHandler(quit);
		return window;
	}

	void destroyWindow(Ptr<Window> const & window)
	{
		windows.queueForErase(window);
		if (!looping) // not looping so we're safe to just erase it.
		{
			windows.processEraseQueue();
		}
	}

	Ptr<world::World> createWorld()
	{
		return *worlds.insertNew();
	}

	void destroyWorld(Ptr<world::World> const & world)
	{
		worlds.queueForErase(world);
	}

	void showMessage(std::string const & message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", message.c_str(), nullptr);
	}
}

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
	try
	{
		// Grab the params. Don't include the 0th arg, because it is the program name.
		std::vector<std::string> args;
		for (int i = 1; i < argc; ++i)
		{
			args.push_back(std::string(argv[i]));
		}

		// Initialize SDL.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}

		// Call the setup.
		ve::userStart(args);

		// Start the loop.
		ve::loop();

		// Call the teardown.
		ve::userStop();

		ve::windows.queueAllForErase();
		ve::windows.processEraseQueue();

		SDL_Quit();

		return 0;
	}
	catch (std::exception const & e)
	{
		ve::showMessage(e.what());
		return -1;
	}
}

