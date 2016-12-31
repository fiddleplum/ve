#pragma once

#include "window.hpp"
#include "store.hpp"
#include "util/ptr.hpp"

namespace ve
{
	//! Starts after the application starts, but before the game loop has started. The user needs to implement this.
	void setup(std::vector<std::string> const & args);

	//! Starts after the game loop has started, but before the application has stopped. The user needs to implement this.
	void teardown();

	//! Stops the ve loop at the end of the current frame.
	void quit();

	//! Sets a function to be called on every update.
	void setUpdateFunction(std::function<void(float dt)> const & function);

	//! Creates a window.
	Ptr<Window> createWindow();

	//! Destroys a window.
	void destroyWindow(Ptr<Window> window);

	//! Shows a message window.
	void showMessage(std::string const & message);
}
