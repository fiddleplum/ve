#pragma once

#include "window.hpp"
#include "store.hpp"
#include "util/ptr.hpp"

//! The user needs to implement the standard int main().

namespace ve
{
	//! Initializes ve. Must be called before all other functions.
	void initialize();

	//! Finalizes ve. Should be called after all other functions.
	void finalize();

	//! Starts the ve loop.
	void loop();

	//! Stops the ve loop at the end of the current frame.
	void quit();

	//! Sets a function to be called on every update.
	void setUpdateFunction(std::function<void(float dt)> const & function);

	//! Creates a window.
	Ptr<Window> createWindow();

	//! Destroys a window.
	void destroyWindow(Ptr<Window> window);
}

// Required for SDL main macro strangeness. This allows the user to just use regular main.
#ifdef __cplusplus
extern "C"
#endif
#define main SDL_main

