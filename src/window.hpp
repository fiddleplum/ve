#pragma once

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

		// Sets the scene to render in this window.
		void setScene(Ptr<render::Scene> const & scene);

		// Gets the SDL Window handle.
		void * getSDLWindow() const;

		// Called by App when it receives the close event.
		void handleCloseEvent();

		// Called by App when it receives the resize event.
		void handleResizeEvent(Vector2i size);

		// Updates the window and the contained gui.
		void update(float dt);

		// Called to render the window.
		void render() const;

	private:
		void * sdlWindow;
		std::function<void()> closeHandler;
		std::function<void(Vector2i size)> resizeHandler;
		OwnPtr<render::WindowStage> stage;
	};
}