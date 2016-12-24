#include "viewport.hpp"
#include "store.hpp"

namespace ve
{
	Viewport::Viewport(Ptr<Scene> scene)
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

	Ptr<TextureStage> Viewport::getStage() const
	{
		return stage;
	}

	//void Viewport::setWorld(Ptr<world3d::World> const & world)
	//{
	//	if (stage->getScene().isValid())
	//	{
	//		stage->getScene()->removeUniformsFunction((int)(uintptr_t)this);
	//	}
	//	stage->setScene(world->getScene());
	//	if (stage->getScene().isValid())
	//	{
	//		stage->getScene()->addUniformsFunction((int)(uintptr_t)this, [this](Ptr<Shader> const & shader)
	//		{

	//		});
	//	}
	//}

	//void Viewport::setCamera(Ptr<world3d::Camera> const & camera)
	//{
	//	stage->getScene()->addUniformsFunction((int)(uintptr_t)this, [camera](Ptr<Shader> const & shader)
	//	{

	//	});
	//}

	void Viewport::update(float dt)
	{
	}
}