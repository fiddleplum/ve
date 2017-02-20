#include "viewport.hpp"

/*

Viewport should not create the target, but should be given it. The Target should also have a setSize(), which sets the size of all its set textures.

What then creates the target?

*/

namespace ve
{
	Viewport::Viewport(Ptr<render::Scene> const & scene, Ptr<render::Shader> const & shader)
		: Widget(scene, shader)
	{
		sprite.setNew(scene, shader);
	}

	Viewport::~Viewport()
	{
		if (target.isValid())
		{
			getScene()->removeDependentTarget(target);
		}
	}

	float Viewport::getDepth() const
	{
		return sprite->getDepth();
	}

	void Viewport::setDepth(float depth)
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
		if (target.isValid())
		{
			target->setSize(bounds.getSize());
		}
	}

	Ptr<render::ImageTarget> Viewport::getTarget() const
	{
		return target;
	}

	void Viewport::setTarget(Ptr<render::ImageTarget> const & target_)
	{
		if (target.isValid())
		{
			getScene()->removeDependentTarget(target);
		}
		target = target_;
		target->setSize(sprite->getBounds().getSize());
		sprite->setImage(target->getColorImage(0));
		getScene()->addDependentTarget(target);
	}

	void Viewport::onCursorPositionChanged(std::optional<Vector2i> cursorPosition)
	{
	}

	void Viewport::update(float dt)
	{
	}
}