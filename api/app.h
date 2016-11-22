#pragma once

#include "window.h"
#include "util/ptr.h"
#include "store.h"

namespace ve
{
	class App
	{
	public:
		// Stops the application loop. The application will quit after the frame is complete.
		virtual void quit() = 0;

		// Creates a window.
		virtual Ptr<Window> createWindow() = 0;

		// Destroys a window.
		virtual void destroyWindow(Ptr<Window> window) = 0;

		// Gets the resource manager.
		virtual Ptr<Store> getStore() const = 0;

	protected:
		// Virtual destructor.
		virtual ~App() = default;
	};

	// Called to get the single instance of the app.
	Ptr<App> getApp();
}