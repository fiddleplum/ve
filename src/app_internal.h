#pragma once

#include "app.h"
#include "window_internal.h"
#include "ptr.h"
#include "object_list.h"
#include "resources_store_internal.h"
#include "render/renderer.h"

namespace ve
{
	class AppInternal : public virtual App
	{
	public:
		// Constructor;
		AppInternal();

		// Starts the application loop. The loop will continue until quit() is called.
		void loop();

		// Stops the application loop. The application will quit.
		void quit() override;

		// Creates a window.
		UsePtr<Window> createWindow() override;

		// Destroys a window.
		void destroyWindow(UsePtr<Window> window) override;

		// Gets the resource manager.
		UsePtr<ResourceStore> getResourceStore() const override;

		// Gets the resource manager.
		UsePtr<ResourceStoreInternal> getResourceStoreInternal() const;

	private:
		void handleSDLEvent(SDL_Event const & sdlEvent);
		UsePtr<WindowInternal> getWindowFromId(unsigned int id);

		bool looping;
		float secondsPerUpdate;
		ObjectList<OwnPtr<WindowInternal>> windows;
		OwnPtr<ResourceStoreInternal> resourceStore;
		OwnPtr<Renderer> renderer;
	};

	// Called to get the single instance of the app.
	UsePtr<AppInternal> getAppInternal();
}