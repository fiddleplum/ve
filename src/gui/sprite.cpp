#include "gui/sprite.hpp"
#include "store.hpp"

namespace ve
{
	Sprite::Sprite(Ptr<Scene> scene)
		: Widget(scene)
	{
		auto vbo = store.vertexBufferObjects.get("guiUnitSquare");
		auto material = store.materials.get("guiMaterial");
		minUniformLocation = material->getShader()->getUniformInfo("min").location;
		maxUniformLocation = material->getShader()->getUniformInfo("max").location;
		model = scene->createModel();
		model->setVertexBufferObject(vbo);
		model->setMaterial(material);
		model->setUniformsFunction([this](Material const & material)
		{
			material.getUniform(minUniformLocation).as<UniformVector2i>()->value = bounds.min;
			material.getUniform(maxUniformLocation).as<UniformVector2i>()->value = bounds.max;
		});
	}

	Sprite::~Sprite()
	{
		getScene()->destroyModel(model);
	}

	float Sprite::getDepth() const
	{
		return model->getDepth();
	}

	void Sprite::setDepth(float depth)
	{
		model->setDepth(depth);
	}

	Recti Sprite::getBounds() const
	{
		return bounds;
	}

	void Sprite::setBounds(Recti bounds_)
	{
		bounds = bounds_;
	}

	void Sprite::setImage(std::string const & name)
	{
		auto & texture = model->getMaterial()->getUniform("tex").as<UniformTexture2d>()->texture;
		texture = store.textures.get(name);
		if (!texture)
		{
			auto image = store.images.get(name);
			if (!image)
			{
				throw std::runtime_error("Image name '" + name + "' not found. ");
			}
			texture = store.textures.create(name, image);
		}	 
	}

	void Sprite::update(float dt)
	{
	}
}