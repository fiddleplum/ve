#pragma once

#include "window.h"
#include "util/ptr.h"
#include "resource_store.h"

namespace ve
{
	class App
	{
	public:
		// Stops the application loop. The application will quit after the frame is complete.
		virtual void quit() = 0;

		// Creates a window.
		virtual UsePtr<Window> createWindow() = 0;

		// Destroys a window.
		virtual void destroyWindow(UsePtr<Window> window) = 0;

		// Gets the resource manager.
		virtual UsePtr<ResourceStore> getResourceStore() const = 0;

	protected:
		// Virtual destructor.
		virtual ~App() = default;
	};

	// Called to get the single instance of the app.
	UsePtr<App> getApp();
}