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
		renderColorImage.setNew(getBounds().getSize(), render::Image::RGB24);
		renderDepthImage.setNew(getBounds().getSize(), render::Image::DEPTH);
		sprite->setImage(renderColorImage);
		target.setNew();
		target->setColorImage(0, renderColorImage);
		target->setDepthImage(renderDepthImage);
		scene->addDependentTarget(target);
	}

	Viewport::~Viewport()
	{
		getScene()->removeDependentTarget(target);
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
		renderColorImage->setSize(bounds.getSize());
		renderDepthImage->setSize(bounds.getSize());
		target->setColorImage(0, renderColorImage);
		target->setDepthImage(renderDepthImage);
	}

	Ptr<render::ImageTarget> Viewport::getTarget() const
	{
		return target;
	}

	void Viewport::onCursorPositionChanged(std::optional<Vector2i> cursorPosition)
	{
	}

	void Viewport::update(float dt)
	{
	}
}