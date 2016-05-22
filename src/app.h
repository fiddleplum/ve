#pragma once

#include "gui/window.h"
#include <set>

//#include <list>
//#include <vector>
//#include <string>
//#include <map>

class Scene;
typedef void * SDL_GLContext;
union SDL_Event;

namespace ve
{
	// Called after the app is opened and before the loop. Implemented by the user.
	void onAppOpen(std::vector<std::string> const & params);

	// Called before the app is closed. Implemented by the user.
	void onAppClose();

	class App
	{
	public:
		// Constructor.
		App();

		// Destructor.
		~App();

		// Quits the application. Stops the loop.
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

		// Called by main to start the loop.
		void loop();

	private:
		void handleSDLEvent(SDL_Event const & event);
		UsePtr<gui::Window> getWindowFromId(unsigned int id) const;

		std::set<OwnPtr<gui::Window>> windows;
		//PtrSet<Scene> scenes;
		bool looping;
		float targetFrameRate;
		SDL_GLContext glContext;
	};

	extern OwnPtr<App> app;
}

