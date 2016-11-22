#pragma once

#include "window.h"
#include "gui/gui_internal.h"
#include "render/renderer.h"
#include "render/stage.h"
#include <SDL.h>

/*

each window has a ptr<stage> as its root stage.
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

		// Sets the function to be called when the window is closed by the user.
		void setCloseHandler(std::function<void()> closeHandler) override;

		// Sets the function to be called when the window is resized by the user.
		void setResizeHandler(std::function<void(Vector2i size)> resizeHandler) override;

		// Gets the root panel element which is the gui.
		Ptr<Gui> getGui() const override;

		// Gets the SDL Window handle.
		SDL_Window * getSDLWindow() const;

		// Called by App when it receives the close event.
		void handleCloseEvent();

		// Called by App when it receives the resize event.
		void handleResizeEvent(Vector2i size);

		// Updates the window and the contained gui.
		void update(float dt);

		// Called to render the window.
		void render(SDL_GLContext glContext) const;

	private:
		SDL_Window * sdlWindow;
		std::function<void()> closeHandler;
		std::function<void(Vector2i size)> resizeHandler;
		OwnPtr<WindowStage> stage;
		OwnPtr<GuiInternal> gui;
	};
}