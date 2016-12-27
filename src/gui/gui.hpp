#pragma once

#include "gui/panel.hpp"
#include "render/scene.hpp"

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

		// Sets the size of the gui.
		void setSize(Vector2i size);

		// Updates the gui.
		void update(float dt);

	private:
		void createResources();

		OwnPtr<Panel> root;
		OwnPtr<Scene> scene;
	};
}