#include "gui_internal.h"

namespace ve
{
	GuiInternal::GuiInternal()
	{
		scene.setNew();
		scene->setUniformsFunction([this](Material const & material)
		{
			Recti bounds = this->getRootPanel()->getBounds();
			material.getUniform("guiSize").as<UniformVector2i>()->value = bounds.max - bounds.min + Vector2i {1, 1};
		});
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

	void GuiInternal::handleResizeEvent(Vector2i size)
	{
		root->setBounds({{0, 0}, {size[0] - 1, size[1] - 1}});
	}

	void GuiInternal::update(float dt)
	{
		root->update(dt);
	}
}