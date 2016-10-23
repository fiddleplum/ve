#pragma once

#include "window.h"
#include "panel_internal.h"
#include <SDL.h>

namespace ve
{
	class WindowInternal : public virtual Window
	{
	public:
		WindowInternal();

		~WindowInternal();

		void setCloseHandler(std::function<bool()> closeHandler) override;

		bool callCloseHandler();

		UsePtr<Panel> getGui() const override;

		SDL_Window * getSDLWindow() const;

		void update(float dt);

	private:
		SDL_Window * sdlWindow;
		std::function<bool()> closeHandler;
		OwnPtr<PanelInternal> gui;
	};
}