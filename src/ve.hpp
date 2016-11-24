#pragma once

#include "window.hpp"
#include "store.hpp"
#include "util/ptr.hpp"

namespace veUser
{
	// Called just after the application opens. Implemented by the user.
	void startup(std::vector<std::string> const & params);

	// Called just before the application quits. Implemented by the user.
	void shutdown();

	// Called once a frame. Implemented by the user.
	void update(float dt);
}

namespace ve
{
	// Stops the application loop at the end of the frame. The application will quit.
	void quit();

	// Creates a window.
	Ptr<Window> createWindow();

	// Destroys a window.
	void destroyWindow(Ptr<Window> window);
}