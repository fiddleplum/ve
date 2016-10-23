#pragma once

#include "window.h"
#include "gui/panel_internal.h"
#include "render/renderer.h"
#include <SDL.h>

namespace ve
{
	class WindowInternal : public virtual Window
	{
	public:
		// Construct the window.
		WindowInternal();

		// Destruct the window.
		~WindowInternal();

		// Sets the function to be called when the window is closed by the user. Returning true destroys the window.
		void setCloseHandler(std::function<bool()> closeHandler) override;

		// Calls the close handler. Called by App when it receives the event.
		bool callCloseHandler();

		// Gets the root panel element which is the gui.
		UsePtr<Panel> getGui() const override;

		// Gets the SDL Window handle.
		SDL_Window * getSDLWindow() const;

		// Updates the window and the contained gui.
		void update(float dt);

		// Called to render the window.
		void render(UsePtr<Renderer> renderer) const;

	private:
		SDL_Window * sdlWindow;
		std::function<bool()> closeHandler;
		OwnPtr<PanelInternal> gui;
	};
}