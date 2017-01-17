#pragma once

#include "window.hpp"
#include "store.hpp"
#include "world/world.hpp"
#include "util/ptr.hpp"

namespace ve
{
	//! Called after the application starts, but before the game loop has started. The user needs to implement this.
	void userStart(std::vector<std::string> const & args);

	//! Called after the game loop has stopped, but before the application has stopped. The user needs to implement this.
	void userStop();

	//! Called once every frame. dt is in seconds.
	void userUpdate(float dt);

	//! Called when the user requests a quit, either by closing the last window, Cmd-Q, Alt-F4, etc.
	void userRequestQuit();

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
}
