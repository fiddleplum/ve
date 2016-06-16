#pragma once

#include "gui/window.h"
#include <set>

//#include <list>
//#include <vector>
//#include <string>
//#include <map>

namespace ve
{
	// Called when the application starts. Implemented by the user. You can open a window and call loop from here.
	void start(std::vector<std::string> const & params);

	// Starts the application game loop.
	void loop();

	// Quits the application. Continues until the end of the current iteration of the loop and then stops.
	void quit();

	// Creates a new window.
	UsePtr<gui::Window> addWindow(std::string const & title);

	// Removes a window.
	void removeWindow(UsePtr<gui::Window> window);

	// Adds a scene.
	//Ptr<Scene> addScene();

	// Removes a scene.
	//void removeScene(Ptr<Scene> scene);

	// Shows a message dialog box.
	void showMessage(std::string const & message);
}

