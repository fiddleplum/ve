#pragma once

#include "window.hpp"
#include "store.hpp"
#include "world/world.hpp"
#include "util/ptr_set.hpp"

// User implements this function.
void entry(std::vector<std::string> const & args);

union SDL_Event;

namespace ve
{
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

	//! Sets the update callback. Called once per frame.
	void setUpateCallback(std::function<void(float dt)> const & callback);

	//! Sets the input event callback. Called once per input event.
	// void setInputEventCallback(std::function<void(InputEvent const & event)> const & callback);

	//! Sets the request quit callback, called when the user requests a quit, either by closing the last window, Cmd-Q, Alt-F4, etc. The user needs to implement this.
	void setRequestQuitCallback(std::function<void()> const & callback);
}

