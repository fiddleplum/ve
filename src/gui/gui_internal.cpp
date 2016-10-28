#include "gui_internal.h"

namespace ve
{
	GuiInternal::GuiInternal()
	{
		scene.setNew();
		root.setNew(scene);
	}

	GuiInternal::~GuiInternal()
	{
		root.setNull();
		scene.setNull();
	}

	UsePtr<Panel> GuiInternal::getRootPanel() const
	{
		return root;
	}

	UsePtr<Scene> GuiInternal::getScene() const
	{
		return scene;
	}

	void GuiInternal::update(float dt)
	{
		root->update(dt);
	}
}