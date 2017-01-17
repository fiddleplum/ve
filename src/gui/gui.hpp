#pragma once

#include "gui/panel.hpp"
#include "render/scene.hpp"

namespace ve
{
	class Gui final
	{
	public:
		// Constructor.
		Gui();

		// Destructor.
		~Gui();

		// Returns the root panel used by the gui.
		Ptr<Panel> getRootPanel() const;

		// Returns the scene used by the gui.
		Ptr<render::Scene> getScene() const;

		// Sets the size of the gui.
		void setSize(Vector2i size);

		// Internal. Called when the user moves the cursor within the gui or out of the gui.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition);

		// Internal. Updates the gui.
		void update(float dt);

	private:
		void createResources();

		OwnPtr<Panel> root;
		OwnPtr<render::Scene> scene;
	};
}