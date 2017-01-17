#include "viewport.hpp"
#include "store.hpp"

namespace ve
{
	Viewport::Viewport(Ptr<render::Scene> scene)
		: Widget(scene)
	{
		sprite.setNew(scene);
		renderTexture.setNew(getBounds().getSize(), Image::RGB24);
		sprite->setTexture(renderTexture);
		target.setNew();
		target->setColorTexture(0, renderTexture);
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
		renderTexture->setSize(bounds.getSize());
	}

	Ptr<render::TextureTarget> Viewport::getTarget() const
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