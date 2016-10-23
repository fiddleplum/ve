#include "window_internal.h"
#include "display.h"
#include "render/open_gl.h"
#include <string>
#include <algorithm>
#include <map>

namespace ve
{
	WindowInternal::WindowInternal(std::string const & title)
	{
		Vector2i initialSize = {800, 600};
		cursorPosition = std::nullopt;
		sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialSize[0], initialSize[1], SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (sdlWindow == nullptr)
		{
			throw std::runtime_error("Failed to create the window.");
		}
	}

	WindowInternal::~WindowInternal()
	{
		root.setNull();
		SDL_DestroyWindow(sdlWindow);
	}

	void WindowInternal::setTitle(std::string const & title)
	{
		SDL_SetWindowTitle(sdlWindow, title.c_str());
	}

	void WindowInternal::setWindowed()
	{
		SDL_SetWindowFullscreen(sdlWindow, 0);
		SDL_EnableScreenSaver();
	}

	void WindowInternal::setFullscreen(int display, Vector2i resolution)
	{
		try
		{
			SDL_DisplayMode mode;
			if (SDL_GetDesktopDisplayMode(display, &mode) < 0)
			{
				throw std::exception();
			}
			mode.w = resolution[0];
			mode.h = resolution[1];
			SDL_DisableScreenSaver();
			if (SDL_SetWindowDisplayMode(sdlWindow, &mode) < 0)
			{
				throw std::exception();
			}
			if (SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
			{
				throw std::exception();
			}
		}
		catch (...)
		{
			throw std::runtime_error("Could not set display " + std::to_string(display) + " to the resolution " + std::to_string(resolution[0]) + "x" + std::to_string(resolution[1]) + ".");
		}
	}

	void WindowInternal::setFullscreen()
	{
		int display = getDisplay();
		setFullscreen(display, Display::getStartingResolution(display));
	}

	Vector2i WindowInternal::getSize() const
	{
		Vector2i size;
		SDL_GetWindowSize(sdlWindow, &size[0], &size[1]);
		return size;
	}

	bool WindowInternal::isFullscreen() const
	{
		return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
	}

	int WindowInternal::getDisplay() const
	{
		int display = SDL_GetWindowDisplayIndex(sdlWindow);
		if (display >= 0)
		{
			return display;
		}
		throw std::runtime_error("Could not get the display the window is within. ");
	}

	UsePtr<gui::Element> WindowInternal::getRootGuiElement()
	{
		return root;
	}

	void handleClose()
	{
	}

	void WindowInternal::handleResize(Vector2i size)
	{
		if (root)
			root->setBounds(Recti {{0, 0}, {size[0], size[1]}});
	}

	void WindowInternal::handleInputEvent(InputEvent const & event)
	{
		if (root)
			root->handleInputEvent(event, cursorPosition);
	}

	void WindowInternal::update(float dt)
	{
		if (root)
			root->update(dt);
	}

	void WindowInternal::preRenderUpdate()
	{
		if (root)
			root->preRenderUpdate();
	}

	void WindowInternal::render(SDL_GLContext glContext) const
	{
		SDL_GL_MakeCurrent(sdlWindow, glContext);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0, 0, 0, 1);
		glClearDepth(-1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Vector2i windowSize = getSize();
		glViewport(0, 0, windowSize[0], windowSize[1]);

		if (root)
			root->render(windowSize);

		SDL_GL_SwapWindow(sdlWindow);
	}

	void WindowInternal::setCursorPosition(std::optional<Vector2i> position)
	{
		cursorPosition = position;
	}

	SDL_Window * WindowInternal::getSDLWindow() const
	{
		return sdlWindow;
	}

	void WindowInternal::setRoot(OwnPtr<gui::Element> element)
	{
		root = element;
		Vector2i windowSize = getSize();
		root->setBounds(Recti {{0, 0}, {windowSize[0], windowSize[1]}});
	}
}

