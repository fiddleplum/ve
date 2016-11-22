#pragma once

#include "gui/gui.h"
#include "util/ptr.h"
#include <functional>

namespace ve
{
	class Window
	{
	public:
		// Sets the function to be called when the window is closed by the user.
		virtual void setCloseHandler(std::function<void()> closeHandler) = 0;

		// Sets the function to be called when the window is closed by the user.
		virtual void setResizeHandler(std::function<void(Vector2i size)> resizeHandler) = 0;

		// Gets the root panel element which is the gui.
		virtual Ptr<Gui> getGui() const = 0;

	protected:
		// Virtual destructor.
		virtual ~Window() = default;
	};
}