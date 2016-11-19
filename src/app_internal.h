#pragma once

#include "app.h"
#include "window_internal.h"
#include "util/ptr.h"
#include "util/object_list.h"
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
		Ptr<Window> createWindow() override;

		// Destroys a window.
		void destroyWindow(Ptr<Window> window) override;

		// Gets the resource manager.
		Ptr<ResourceStore> getResourceStore() const override;

		// Gets the resource manager.
		Ptr<ResourceStoreInternal> getResourceStoreInternal() const;

	private:
		void handleSDLEvent(SDL_Event const & sdlEvent);
		Ptr<WindowInternal> getWindowFromId(unsigned int id);

		bool looping;
		float secondsPerUpdate;
		ObjectList<OwnPtr<WindowInternal>> windows;
		OwnPtr<ResourceStoreInternal> resourceStore;
		OwnPtr<Renderer> renderer;
	};

	// Called to get the single instance of the app.
	Ptr<AppInternal> getAppInternal();
}