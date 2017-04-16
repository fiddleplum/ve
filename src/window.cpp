#include "window.hpp"
#include <SDL.h>
#include <stdexcept>

namespace ve
{
	Window::Window()
	{
		// Initialize SDL.
		Vector2i initialSize {800, 600};
		sdlWindow = SDL_CreateWindow("Untitled", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialSize[0], initialSize[1], SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (sdlWindow == nullptr)
		{
			throw std::runtime_error("Failed to create the window.");
		}

		// Create a new window target.
		target.setNew(sdlWindow);

		// Create the gui and connect it to the window target.
		gui.setNew();
		gui->setSize(initialSize);
		target->setScene(gui->getScene());
	}

	Window::~Window()
	{
		gui.setNull();
		target.setNull();
		SDL_DestroyWindow((SDL_Window *)sdlWindow);
	}

	Ptr<Gui> Window::getGui() const
	{
		return gui;
	}

	void Window::setCloseRequestedHandler(std::function<void()> closeRequestedHandler_)
	{
		closeRequestedHandler = closeRequestedHandler_;
	}

	void * Window::getSDLWindow() const
	{
		return sdlWindow;
	}

	void Window::onCloseRequested()
	{
		if (closeRequestedHandler)
		{
			closeRequestedHandler();
		}
	}

	void Window::onResized(Vector2i size)
	{
		gui->setSize(size);
	}

	void Window::onCursorPositionChanged(std::optional<Vector2i> cursorPosition_)
	{
		cursorPosition = cursorPosition_;
		gui->onCursorPositionChanged(cursorPosition);
	}

	void Window::update(float dt)
	{
		gui->update(dt);
	}

	void Window::preRender()
	{
		target->clearRenderedThisFrameFlag();
	}

	void Window::render() const
	{
		target->render();
	}
}