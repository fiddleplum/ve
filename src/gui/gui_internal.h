#pragma once

#include "gui/gui.h"
#include "gui/panel_internal.h"
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
		Ptr<Panel> getRootPanel() const override;

		// Returns the scene used by the gui.
		Ptr<Scene> getScene() const;

		// Handles a resize event.
		void handleResizeEvent(Vector2i size);

		// Updates the gui.
		void update(float dt);

	private:
		OwnPtr<PanelInternal> root;
		OwnPtr<Scene> scene;
	};
}