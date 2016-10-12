#pragma once

#include "app.h"

namespace ve
{
	class AppInternal : public App
	{
	public:
		// Starts the application game loop.
		void loop();

		UsePtr<Window> createWindow(std::string const & title) override;

		void destroyWindow(UsePtr<Window> window) override;

		UsePtr<scene::Scene> createScene() override;

		void destroyScene(UsePtr<scene::Scene> scene) override;

		void showMessage(std::string const & message) override;
	};
}

