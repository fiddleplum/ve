#include "gui.hpp"

namespace ve
{
	Gui::Gui()
	{
		scene.setNew();
		scene->setUniformsFunction([this](Material const & material)
		{
			Recti bounds = this->getRootPanel()->getBounds();
			material.getUniform("guiSize").as<UniformVector2i>()->value = bounds.max - bounds.min + Vector2i {1, 1};
		});
		root.setNew(scene);
	}

	Gui::~Gui()
	{
		root.setNull();
		scene.setNull();
	}

	Ptr<Panel> Gui::getRootPanel() const
	{
		return root;
	}

	Ptr<Scene> Gui::getScene() const
	{
		return scene;
	}

	void Gui::handleResizeEvent(Vector2i size)
	{
		root->setBounds({{0, 0}, {size[0] - 1, size[1] - 1}});
	}

	void Gui::update(float dt)
	{
		root->update(dt);
	}
}