#pragma once

#include "gui/gui.hpp"
#include "render/renderer.hpp"
#include "render/stage.hpp"

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
	class Window final
	{
	public:
		// Construct the window.
		Window();

		// Destruct the window.
		~Window();

		// Sets the function to be called when the window is closed by the user.
		void setCloseHandler(std::function<void()> closeHandler);

		// Sets the function to be called when the window is resized by the user.
		void setResizeHandler(std::function<void(Vector2i size)> resizeHandler);

		// Gets the root panel element which is the gui.
		Ptr<Gui> getGui() const;

		// Gets the SDL Window handle.
		void * getSDLWindow() const;

		// Called by App when it receives the close event.
		void handleCloseEvent();

		// Called by App when it receives the resize event.
		void handleResizeEvent(Vector2i size);

		// Updates the window and the contained gui.
		void update(float dt);

		// Called to render the window.
		void render(void * glContext) const;

	private:
		void * sdlWindow;
		std::function<void()> closeHandler;
		std::function<void(Vector2i size)> resizeHandler;
		OwnPtr<WindowStage> stage;
		OwnPtr<Gui> gui;
	};
}