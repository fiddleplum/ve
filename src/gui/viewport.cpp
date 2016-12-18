#include "viewport.hpp"

namespace ve
{
	Viewport::Viewport(Ptr<Stage> guiStage_, Ptr<Scene> scene)
		: Widget(scene)
	{
		guiStage = guiStage_;
		sprite.setNew(scene);
		stage.setNew();
		guiStage->addPriorStage(stage);
		sprite->setTexture(stage->getColorTarget(0));
	}

	Viewport::~Viewport()
	{
		guiStage->removePriorStage(stage);
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