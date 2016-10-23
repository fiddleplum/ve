#pragma once

#include "vector.h"
#include <string>

namespace ve
{
	class Window
	{
	public:
		// Sets the title of the window.
		virtual void setTitle(std::string const & title) = 0;

		// Sets the window in windowed mode, with borders and title bar.
		virtual void setWindowed() = 0;

		// Sets the window in fullscreen mode on the given display at the given resolution.
		virtual void setFullscreen(int display, Vector2i resolution) = 0;

		// Sets the window in fullscreen mode on the current display at the current resolution of that display.
		virtual void setFullscreen() = 0;

		// Returns the size of the window, not including borders and title bar.
		virtual Vector2i getSize() const = 0;

		// Returns whether or not the window is in fullscreen mode.
		virtual bool isFullscreen() const = 0;

		// Returns the current display containing the window.
		virtual int getDisplay() const = 0;

		//// Gets the root gui element.
		//UsePtr<gui::Element> getRootGuiElement();

		//// Sets the root gui element. The bounds of the element will always be set to the bounds of the window.
		//template <typename T> UsePtr<T> setRootGuiElement();

		// Destructor.
		virtual ~Window() = default;
	};
}

