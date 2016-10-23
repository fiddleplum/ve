#pragma once

#include "window.h"
#include "gui/element.h"
#include "event.h"
#include "ptr.h"
#include <vector>
#include <SDL.h>

// QUESTION:
/*
I'm working on how to set the size of the root element when it is just a guielement, not a container.
Should window contain a container or should it be a container or contain a guielement?
Should button inherit from sprite rather than contain it?
Should there be size events or just a setSize function, and should it be in guielement?

Option:
Make Window have a container element.
This means that layer must inherit from container to be a root element.
This means that container must have its functions protected.
This means that all other container-type elements must inherit rather than contain container.

Option:
Make all elements have a setSize function.
This means that things like button and sprite need to do nothing when it is called.

Option:
Make resizing an event.
This means that positioning should also be an event for consistency.

*/

namespace ve
{
	class WindowInternal : public Window
	{
	public:
		// Constructor. Sets the title and starts in windowed mode at 800x600 in the center of the screen.
		WindowInternal(std::string const & title);

		virtual ~WindowInternal();

		void setTitle(std::string const & title) override;

		void setWindowed() override;

		void setFullscreen(int display, Vector2i resolution) override;

		void setFullscreen() override;

		Vector2i getSize() const;

		bool isFullscreen() const;

		int getDisplay() const;

		//UsePtr<gui::Element> getRootGuiElement();

		//template <typename T> UsePtr<T> setRootGuiElement();

		// Called when the window is resized.
		void handleResize(Vector2i size);

		// Called when an event comes in.
		void handleInputEvent(InputEvent const & event);

		// Called to update the window gui elements.
		void update(float dt);

		// Called to update the window elements just before the render.
		void preRenderUpdate();

		// Called to render the window.
		void render(SDL_GLContext glContext) const;

		// Called to set the position of the cursor relative to the window.
		void setCursorPosition(std::optional<Vector2i> position);

		// Called to get the SDL handle for the window.
		SDL_Window * getSDLWindow() const;

	private:
		void setRoot(OwnPtr<gui::Element> element);

		SDL_Window * sdlWindow;
		OwnPtr<gui::Element> root;
		std::optional<Vector2i> cursorPosition;
	};

	template <typename T> UsePtr<T> Window::setRootGuiElement()
	{
		OwnPtr<T> element = OwnPtr<T>::createNew();
		setRoot(element);
		return element;
	}
}

