#pragma once

#include "container.h"
#include "../event.h"
#include <vector>

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

struct SDL_Window;
union SDL_Event;
typedef void * SDL_GLContext;

namespace ve
{
	namespace gui
	{
		class Window
		{
		public:
			Window(std::string const & title);

			~Window();

			void setTitle(std::string const & title);

			UsePtr<Element> getRoot();

			template <typename T> UsePtr<T> setRoot();

			void setWindowed();

			void setFullscreen(int display, Vector2i size);

			void setFullscreen();

			Vector2i getSize() const;

			bool isFullscreen() const;

			int getDisplay() const;

			// Called by App when the window is resized.
			void handleResize(Vector2i size);

			// Called by App when an event comes in.
			void handleEvent(Event const & event);

			// Called by App to update the window gui elements.
			void update(float dt);

			// Called by App to update the window elements just before the render.
			void preRenderUpdate();

			// Called by App to render the window.
			void render(SDL_GLContext glContext) const;

			// Called by App to set the position of the cursor relative to the window.
			void setCursorPosition(std::optional<Vector2i> position);

			// Called by App to get the SDL handle for the window.
			SDL_Window * getSDLWindow() const;

		private:
			SDL_Window * sdlWindow;
			OwnPtr<Container> root;
			std::optional<Vector2i> cursorPosition;
		};

		template <typename T> UsePtr<T> Window::setRoot()
		{
			OwnPtr<T> elementDerived = OwnPtr<T>::createNew();
			root = elementDerived;
			root->setPosition(Vector2i(0, 0));
			root->setSize(getSize());
			return elementDerived;
		}
	}
}

