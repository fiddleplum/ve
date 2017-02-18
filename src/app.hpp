#pragma once

#include "window.hpp"
#include "world/world.hpp"
#include "store.hpp"
#include "input_event.hpp"
#include "util/ptr_set.hpp"

union SDL_Event;

namespace ve
{
	class App final
	{
	public:
		//! Constructs the app.
		App();

		//! Destructs the app.
		virtual ~App();

		//! Starts the user loop. It keeps going until quit is called, or the last window is closed.
		void loop();

		//! Stops the ve loop at the end of the current frame.
		void quit();

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

		//! Returns the store.
		Ptr<Store> getStore() const;

		//! Sets the quit callback. Called right after the game loop exits. Use this to clean up your application.
		void setQuitCallback(std::function<void()> const & callback);

		//! Sets the update callback. Called once per frame.
		void setUpdateCallback(std::function<void(float secondsPerFrame)> const & callback);

		//! Sets the input event callback. Called once per input event.
		void setInputEventCallback(std::function<void(Input::Event const & event)> const & callback);

		//! Sets the request quit callback, called when the user requests a quit, either by closing the last window, Cmd-Q, Alt-F4, etc. The user needs to implement this.
		void setRequestQuitCallback(std::function<void()> const & callback);

	private:
		Ptr<Window> getWindowFromId(unsigned int id);
		void handleSDLEvent(SDL_Event const & sdlEvent);

		bool looping = false;
		float secondsPerUpdate = 1.f / 24.f;
		OwnPtr<Input> input;
		OwnPtr<Store> store;
		PtrSet<Window> windows;
		PtrSet<world::World> worlds;

		std::function<void()> quitCallback;
		std::function<void(float secondsPerFrame)> updateCallback;
		std::function<void(Input::Event const & event)> inputEventCallback;
		std::function<void()> requestQuitCallback;
	};
}

