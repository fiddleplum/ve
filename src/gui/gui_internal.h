#pragma once

#include "gui.h"
#include "panel_internal.h"
#include "render/scene.h"

namespace ve
{
	class GuiInternal : public virtual Gui
	{
	public:
		// Constructor.
		GuiInternal();

		// Destructor.
		~GuiInternal();

		// Returns the root panel used by the gui.
		UsePtr<Panel> getRootPanel() const override;

		// Returns the scene used by the gui.
		UsePtr<Scene> getScene() const;

		// Updates the gui.
		void update(float dt);

	private:
		OwnPtr<PanelInternal> root;
		OwnPtr<Scene> scene;
	};
}