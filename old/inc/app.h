#pragma once

#include "ptr.h"
#include <string>

//#include "window.h"
//#include "scene/scene.h"
//#include <set>

//#include <list>
//#include <vector>
//#include <map>

namespace ve
{
	class App
	{
	public:
		// Quits the application. Continues until the end of the current iteration of the loop and then stops.
		virtual void quit() = 0;

		// Creates a new window.
		//virtual UsePtr<Window> createWindow(std::string const & title) = 0;

		// Removes a window.
		//virtual void destroyWindow(UsePtr<Window> window) = 0;;

		// Adds a scene.
		//virtual UsePtr<scene::Scene> createScene() = 0;

		// Removes a scene.
		//virtual void destroyScene(UsePtr<scene::Scene> scene) = 0;

		// Shows a message dialog box.
		virtual void showMessage(std::string const & message) = 0;
	};
}

