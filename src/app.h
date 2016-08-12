#pragma once

#include "window.h"
#include "scene/scene.h"
#include <set>

//#include <list>
//#include <vector>
//#include <string>
//#include <map>

namespace ve
{
	// Called when the application starts. Implemented by the user. You can open a window and call loop from here.
	void main(std::vector<std::string> const & params);

	// Starts the application game loop.
	void loop();

	// Quits the application. Continues until the end of the current iteration of the loop and then stops.
	void quit();

	// Creates a new window.
	UsePtr<Window> addWindow(std::string const & title);

	// Removes a window.
	void removeWindow(UsePtr<Window> window);

	// Adds a scene.
	UsePtr<scene::Scene> addScene();

	// Removes a scene.
	void removeScene(UsePtr<scene::Scene> scene);

	// Shows a message dialog box.
	void showMessage(std::string const & message);
}

