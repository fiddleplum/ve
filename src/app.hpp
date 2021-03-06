#pragma once

#include "window.hpp"
#include "world/world.hpp"
#include "input.hpp"
#include "util/ptr_set.hpp"

union SDL_Event;

namespace ve
{
	class App final
	{
	public:
		//! Constructs the app.
		App(Config const & config);

		//! Destructs the app.
		virtual ~App();

		//! Starts the user loop. It keeps going until quit is called, or the last window is closed.
		void loop();

		//! Stops the ve loop at the end of the current frame.
		void quit();

		// Returns the interval in seconds over which an update will occur.
		float getSecondsPerUpdate() const;

		// Sets the interval in seconds over which an update will occur.
		void setSecondsPerUpdate(float dt);

		// Returns the iterval in seconds over which a single loop (one frame render) will occur.
		float getSecondsPerLoop() const;

		//! Creates a window.
		Ptr<Window> createWindow();

		//! Destroys a window.
		void destroyWindow(Ptr<Window> const & window);

		//! Creates a world.
		Ptr<world::World> createWorld();

		//! Destroys a world.
		void destroyWorld(Ptr<world::World> const & world);

		//! Shows a message window.
		void showMessage(std::string const & message);

		//! Returns the input system.
		Ptr<Input> getInput() const;

		//! Sets the quit callback. Called right after the game loop exits. Use this to clean up your application.
		void setQuitCallback(std::function<void()> const & callback);

		//! Sets the update callback. Called once per frame. Use this for animations and per frame movements.
		void setUpdateCallback(std::function<void(float secondsPerFrame)> const & callback);

		//! Sets the input event callback. Called once per input event. Use this to handle user input.
		void setInputEventCallback(std::function<void(InputEvent const & event)> const & callback);

		//! Sets the request quit callback, called when the user requests a quit, either by closing the last window, Cmd-Q, Alt-F4, etc. Use this for "Are you sure you want to quit?" functions.
		void setRequestQuitCallback(std::function<void()> const & callback);

	private:
		Ptr<Window> getWindowFromId(unsigned int id);
		void handleSDLEvent(SDL_Event const & sdlEvent);

		bool looping = false;
		float secondsPerUpdate = 1.f / 24.f;
		float secondsPerLoop = 0;
		OwnPtr<Input> input;
		PtrSet<Window> windows;
		PtrSet<world::World> worlds;

		std::function<void()> quitCallback;
		std::function<void(float secondsPerFrame)> updateCallback;
		std::function<void(InputEvent const & event)> inputEventCallback;
		std::function<void()> requestQuitCallback;
	};
}

