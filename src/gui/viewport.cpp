#include "viewport.hpp"
#include "store.hpp"

namespace ve
{
	Viewport::Viewport(Ptr<render::Scene> scene)
		: Widget(scene)
	{
		sprite.setNew(scene);
		renderTarget.setNew(getBounds().getSize(), Image::RGB24);
		sprite->setTexture(renderTarget);
		stage.setNew();
		stage->setColorTarget(0, renderTarget);
		scene->addDependentStage(stage);
		sprite->setTexture(stage->getColorTarget(0));
	}

	Viewport::~Viewport()
	{
		getScene()->removeDependentStage(stage);
	}

	float Viewport::getDepth() const
	{
		return sprite->getDepth();
	}

	void Viewport::setDepth(float & depth)
	{
		sprite->setDepth(depth);
	}

	Recti Viewport::getBounds() const
	{
		return sprite->getBounds();
	}

	void Viewport::setBounds(Recti bounds)
	{
		sprite->setBounds(bounds);
		renderTarget->setSize(bounds.getSize());
	}

	Ptr<render::TextureStage> Viewport::getStage() const
	{
		return stage;
	}

	void Viewport::update(float dt)
	{
	}
}