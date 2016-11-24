#pragma once

#include "window.h"
#include "util/ptr.h"
#include "util/object_list.h"
#include "store.h"
#include "render/renderer.h"

union SDL_Event;

namespace ve
{
	class App final
	{
	public:
		// Constructor;
		App();

		// Starts the application loop. The loop will continue until quit() is called.
		void loop();

		// Stops the application loop. The application will quit.
		void quit();

		// Creates a window.
		Ptr<Window> createWindow();

		// Destroys a window.
		void destroyWindow(Ptr<Window> window);

		// Gets the resource manager.
		Ptr<Store> getStore() const;

	private:
		void handleSDLEvent(SDL_Event const & sdlEvent);
		Ptr<Window> getWindowFromId(unsigned int id);

		bool looping;
		float secondsPerUpdate;
		ObjectList<OwnPtr<Window>> windows;
		OwnPtr<Store> store;
		OwnPtr<Renderer> renderer;
	};

	// Called to get the single instance of the app.
	Ptr<App> getApp();
}