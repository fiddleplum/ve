#pragma once

#include "app.h"
#include <SDL.h>

namespace ve
{
	namespace internal
	{
		class App : public ve::App
		{
		public:
			// Starts the application game loop.
			void loop();

			//UsePtr<Window> createWindow(std::string const & title) override;

			//void destroyWindow(UsePtr<Window> window) override;

			//UsePtr<scene::Scene> createScene() override;

			//void destroyScene(UsePtr<scene::Scene> scene) override;

			void showMessage(std::string const & message) override;

		private:
			void handleSDLEvent(SDL_Event const & event);
			//UsePtr<WindowInternal> getWindowFromId(unsigned int id);

			//std::set<OwnPtr<WindowInternal>> windows;
			//std::set<OwnPtr<scene::Scene>> scenes;
			bool looping;
			float targetFrameRate = 60.f;
			SDL_GLContext glContext = nullptr;
		};
	}
}

