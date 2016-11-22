#include "window_internal.h"
#include "app_internal.h"
#include <stdexcept>

namespace ve
{
	WindowInternal::WindowInternal()
	{
		Vector2i initialSize {800, 600};
		sdlWindow = SDL_CreateWindow("Untitled", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialSize[0], initialSize[1], SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (sdlWindow == nullptr)
		{
			throw std::runtime_error("Failed to create the window.");
		}

		gui.setNew();
		gui->handleResizeEvent(initialSize);

		stage.setNew();
		stage->setWindowSize(initialSize);
		stage->setScene(gui->getScene());
	}

	WindowInternal::~WindowInternal()
	{
		stage.setNull();
		SDL_DestroyWindow(sdlWindow);
	}

	void WindowInternal::setCloseHandler(std::function<void()> closeHandler_)
	{
		closeHandler = closeHandler_;
	}

	void WindowInternal::setResizeHandler(std::function<void(Vector2i size)> resizeHandler_)
	{
		resizeHandler = resizeHandler_;
	}

	Ptr<Gui> WindowInternal::getGui() const
	{
		return gui;
	}

	SDL_Window * WindowInternal::getSDLWindow() const
	{
		return sdlWindow;
	}

	void WindowInternal::handleCloseEvent()
	{
		if (closeHandler)
		{
			closeHandler();
		}
	}

	void WindowInternal::handleResizeEvent(Vector2i size)
	{
		if (resizeHandler)
		{
			resizeHandler(size);
		}
		gui->handleResizeEvent(size);
		stage->setWindowSize(size);
	}

	void WindowInternal::update(float dt)
	{
		gui->update(dt);
	}

	void WindowInternal::render(SDL_GLContext glContext) const
	{
		SDL_GL_MakeCurrent(sdlWindow, glContext);
		stage->render();
		SDL_GL_SwapWindow(sdlWindow);
	}
}