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

	Ptr<Panel> GuiInternal::getRootPanel() const
	{
		return root;
	}

	Ptr<Scene> GuiInternal::getScene() const
	{
		return scene;
	}

	void GuiInternal::update(float dt)
	{
		root->update(dt);
	}
}