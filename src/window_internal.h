#pragma once

#include "window.h"
#include "gui/gui_internal.h"
#include "render/renderer.h"
#include "render/stage.h"
#include <SDL.h>

/*

each window has a useptr<stage> as its root stage.
each stage has a list of dependencies:
	addpriorstage()
	removepriorstage()
	clearpriorstages()

render:
	go through all stages and clear the 'rendered' flag
	for each window, render its top level stage
		do a depth first rendering of each stage
			if the 'rendered' flag isn't set
				render
				set 'rendered' flag
			else
				skip stage

	each stage has a scene and target
		how can i get cameras and lights into the stage? stage shouldn't know about anything 3d
*/

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
		UsePtr<Gui> getGui() const override;

		// Gets the SDL Window handle.
		SDL_Window * getSDLWindow() const;

		// Updates the window and the contained gui.
		void update(float dt);

		// Called to render the window.
		void render(SDL_GLContext glContext) const;

	private:
		SDL_Window * sdlWindow;
		std::function<bool()> closeHandler;
		OwnPtr<WindowStage> stage;
		OwnPtr<GuiInternal> gui;
	};
}