#pragma once

#include "window.hpp"
#include "world/world.hpp"
#include "store.hpp"
#include "input_event.hpp"
#include "util/ptr_set.hpp"

union SDL_Event;

namespace ve
{
	class App
	{
	public:
		//! Constructs the app.
		App(std::vector<std::string> const & args);

		//! Destructs the app.
		virtual ~App();

		//! Starts the user loop. It keeps going until quit is called, or the last window is closed.
		void loop();

		//! Stops the ve loop at the end of the current frame.
		void quit();

		//! Creates a window.
		template <typename WindowType>
		Ptr<WindowType> createWindow();

		//! Destroys a window.
		void destroyWindow(Ptr<Window> const & window);

		//! Creates a world.
		Ptr<world::World> createWorld();

		//! Destroys a world.
		void destroyWorld(Ptr<world::World> const & world);

		//! Shows a message window.
		void showMessage(std::string const & message);

		//! Returns the input system.
		Ptr<Input> getInput() const;

		//! Returns the store.
		Ptr<Store> getStore() const;

		//! Sets the input event callback. Called once per input event.
		// void setInputEventCallback(std::function<void(InputEvent const & event)> const & callback);

		//! Sets the request quit callback, called when the user requests a quit, either by closing the last window, Cmd-Q, Alt-F4, etc. The user needs to implement this.
		void setRequestQuitCallback(std::function<void()> const & callback);

	protected:
		//! Called once per time step to update the app.
		virtual void update(float dt) {}

		//! Called when the user requests a quit, either by closing the last window, Cmd-Q, Alt-F4, etc.
		virtual void requestQuit() {}

		//! Called once per input event.
		virtual void handleInputEvent(Input::Event const & inputEvent) {}

	private:
		Ptr<Window> getWindowFromId(unsigned int id);
		void handleSDLEvent(SDL_Event const & sdlEvent);

		bool looping = false;
		float secondsPerUpdate = 1.f / 24.f;
		OwnPtr<Input> input;
		OwnPtr<Store> store;
		PtrSet<Window> windows;
		PtrSet<world::World> worlds;
	};

	template <typename WindowType>
	Ptr<WindowType> App::createWindow()
	{
		auto window = *windows.insertNew<WindowType>();
		return window;
	}
}

