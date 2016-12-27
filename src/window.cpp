#include "window.hpp"
#include "render/open_gl.hpp"
#include <SDL.h>
#include <stdexcept>

namespace ve
{
	Window::Window()
	{
		Vector2i initialSize {800, 600};
		sdlWindow = SDL_CreateWindow("Untitled", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialSize[0], initialSize[1], SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (sdlWindow == nullptr)
		{
			throw std::runtime_error("Failed to create the window.");
		}

		stage.setNew(sdlWindow);
		stage->setWindowSize(initialSize);

		gui.setNew();
		gui->setSize(initialSize);
		stage->setScene(gui->getScene());
	}

	Window::~Window()
	{
		stage.setNull();
		SDL_DestroyWindow((SDL_Window *)sdlWindow);
	}

	void Window::setCloseHandler(std::function<void()> closeHandler_)
	{
		closeHandler = closeHandler_;
	}

	void Window::setResizeHandler(std::function<void(Vector2i size)> resizeHandler_)
	{
		resizeHandler = resizeHandler_;
	}

	Ptr<Gui> Window::getGui() const
	{
		return gui;
	}

	void * Window::getSDLWindow() const
	{
		return sdlWindow;
	}

	void Window::handleCloseEvent()
	{
		if (closeHandler)
		{
			closeHandler();
		}
	}

	void Window::handleResizeEvent(Vector2i size)
	{
		if (resizeHandler)
		{
			resizeHandler(size);
		}
		gui->setSize(size);
		stage->setWindowSize(size);
	}

	void Window::update(float dt)
	{
		gui->update(dt);
	}

	void Window::render() const
	{
		stage->render();
	}
}