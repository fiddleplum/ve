#pragma once

#include "render/target.hpp"
#include "gui/gui.hpp"

namespace ve
{
	class render::Target;
	class Gui;

	class Window final
	{
	public:
		// Construct the window.
		Window();

		// Destruct the window.
		~Window();

		// Returns the gui.
		Ptr<Gui> getGui() const;

		// Sets the function to be called when the user has requested to close the window.
		void setCloseRequestedHandler(std::function<void()> closeRequestedHandler);

		// Internal. Gets the SDL Window handle.
		void * getSDLWindow() const;

		// Internal. Called when the user clicks the close button on the window or equivalent shortcut.
		void onCloseRequested();

		// Internal. Called when the user resizes the window.
		void onResized(Vector2i size);

		// Internal. Called when the user moves the cursor within the window or out of the window.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition);

		// Internal. Updates the window and the contained gui.
		void update(float dt);

		// Internal. Called to render the window.
		void render() const;

	private:
		void * sdlWindow;
		std::function<void()> closeRequestedHandler;
		OwnPtr<render::WindowTarget> target;
		std::optional<Vector2i> cursorPosition;
		OwnPtr<Gui> gui;
	};
}