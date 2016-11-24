#pragma once

#include "gui/panel.h"
#include "render/scene.h"

namespace ve
{
	class Gui
	{
	public:
		// Constructor.
		Gui();

		// Destructor.
		~Gui();

		// Returns the root panel used by the gui.
		Ptr<Panel> getRootPanel() const;

		// Returns the scene used by the gui.
		Ptr<Scene> getScene() const;

		// Handles a resize event.
		void handleResizeEvent(Vector2i size);

		// Updates the gui.
		void update(float dt);

	private:
		OwnPtr<Panel> root;
		OwnPtr<Scene> scene;
	};
}