#include "window_internal.h"
#include "app_internal.h"
#include <stdexcept>

namespace ve
{
	WindowInternal::WindowInternal()
	{
		sdlWindow = SDL_CreateWindow("Untitled", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (sdlWindow == nullptr)
		{
			throw std::runtime_error("Failed to create the window.");
		}

		gui.setNew();

		stage.setNew();
		stage->setScene(gui->getScene());
	}

	WindowInternal::~WindowInternal()
	{
		SDL_DestroyWindow(sdlWindow);
	}

	void WindowInternal::setCloseHandler(std::function<bool()> closeHandler_)
	{
		closeHandler = closeHandler_;
	}

	bool WindowInternal::callCloseHandler()
	{
		if (closeHandler)
		{
			return closeHandler();
		}
		return true;
	}

	UsePtr<Gui> WindowInternal::getGui() const
	{
		return gui;
	}

	SDL_Window * WindowInternal::getSDLWindow() const
	{
		return sdlWindow;
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