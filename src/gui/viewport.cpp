#include "viewport.hpp"

namespace ve
{
	Viewport::Viewport(Ptr<Scene> scene)
		: Widget(scene)
	{
		sprite.setNew(scene);
		stage.setNew();
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
	}

	Ptr<TextureStage> Viewport::getStage() const
	{
		return stage;
	}

	void Viewport::setWorld(Ptr<world3d::World> const & world)
	{
		if (stage->getScene().isValid())
		{
			stage->getScene()->removeUniformsFunction((int)this);
		}
		stage->setScene(world->getScene());
		if (stage->getScene().isValid())
		{
			stage->getScene()->addUniformsFunction((int)this, [this](Ptr<Shader> const & shader)
			{

			});
		}
	}

	void Viewport::update(float dt)
	{
	}
}