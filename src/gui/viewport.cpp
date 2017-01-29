#include "viewport.hpp"

namespace ve
{
	Viewport::Viewport(Ptr<render::Scene> const & scene, Ptr<render::Shader> const & shader)
		: Widget(scene, shader)
	{
		sprite.setNew(scene, shader);
		renderImage.setNew(getBounds().getSize(), render::Image::RGB24);
		sprite->setImage(renderImage);
		target.setNew();
		target->setColorImage(0, renderImage);
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
		renderImage->setSize(bounds.getSize());
		target->setColorImage(0, renderImage);
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