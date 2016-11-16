#pragma once

#include "gui/gui.h"
#include "util/ptr.h"
#include <functional>

namespace ve
{
	class Window
	{
	public:
		// Sets the function to be called when the window is closed by the user. Returning true destroys the window.
		virtual void setCloseHandler(std::function<bool()> closeHandler) = 0;

		// Gets the root panel element which is the gui.
		virtual UsePtr<Gui> getGui() const = 0;

	protected:
		// Virtual destructor.
		virtual ~Window() = default;
	};
}