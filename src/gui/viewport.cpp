#include "viewport.hpp"

namespace ve
{
	Viewport::Viewport(Ptr<Scene> scene)
		: Widget(scene)
	{
		sprite.setNew(scene);
	}

	// Destructor.
	Viewport::~Viewport()
	{
	}

	// Returns the depth.
	float Viewport::getDepth() const
	{
		return sprite->getDepth();
	}

	// Sets the depth.
	void Viewport::setDepth(float & depth)
	{
		sprite->setDepth(depth);
	}

	// Returns the bounds.
	Recti Viewport::getBounds() const
	{
		return sprite->getBounds();
	}

	// Sets the bounds of the sprite.
	void Viewport::setBounds(Recti bounds)
	{
		sprite->setBounds(bounds);
	}

	// Gets the currently used texture stage.
	Ptr<TextureStage> Viewport::getStage() const
	{
		return stage;
	}

	void Viewport::setStage(Ptr<TextureStage> stage_)
	{
		stage = stage_;
		if (stage.isValid())
		{
			sprite->setTexture(stage->getColorTarget(0));
		}
	}

	void Viewport::update(float dt)
	{
	}
}