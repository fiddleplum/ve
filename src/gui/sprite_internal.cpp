#include "sprite_internal.h"
#include "app_internal.h"

namespace ve
{
	SpriteInternal::SpriteInternal(UsePtr<Scene> scene)
		: WidgetInternal(scene)
	{
		std::string modelName = "guiUnitSquare";
		auto store = getAppInternal()->getResourceStoreInternal();
		auto vbo = store->getVertexBufferObject(modelName);
		if (!vbo)
		{
			auto mesh = store->getMesh(modelName);
			if (!mesh)
			{
				mesh = store->createMesh(modelName);
				mesh->setFormatTypes({Mesh::POSITION_2D, Mesh::UV0});
				mesh->setNumIndicesPerPrimitive(3);
				mesh->setVertices({0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1});
				mesh->setIndices({0, 1, 2, 2, 3, 0});
			}
			vbo = store->createVertexBufferObject(modelName, mesh);
		}
		auto material = store->getMaterial("guiMaterial");
		if (!material)
		{
			auto shader = store->getShader("guiShader");
			if (!shader)
			{
				shader = store->loadShader("guiShader");
			}
			material = store->createMaterial("guiMaterial");
			material->setShader(shader);
			minUniformLocation = material->getUniform("min")->getLocation();
			maxUniformLocation = material->getUniform("max")->getLocation();
		}
		model = scene->createModel();
		model->setVertexBufferObject(vbo);
		model->setMaterial(material);
		model->setUniformsFunction([this]()
		{
			model->getMaterial()->getUniform(minUniformLocation).as<UniformCoord2i>()->value = bounds.min;
			model->getMaterial()->getUniform(maxUniformLocation).as<UniformCoord2i>()->value = bounds.max;
		});
	}

	SpriteInternal::~SpriteInternal()
	{
		getScene()->destroyModel(model);
	}

	float SpriteInternal::getDepth() const
	{
		return model->getDepth();
	}

	void SpriteInternal::setDepth(float depth)
	{
		model->setDepth(depth);
	}

	void SpriteInternal::setBounds(Recti bounds_)
	{
		bounds = bounds_;
	}

	void SpriteInternal::setImage(std::string const & name)
	{
		auto store = getAppInternal()->getResourceStoreInternal();
		auto & texture = model->getMaterial()->getUniform("texture").as<UniformTexture2d>()->texture;
		texture = store->getTexture(name);
		if (!texture)
		{
			auto image = store->getImage(name);
			if (!image)
			{
				throw std::runtime_error("Image name '" + name + "' not found. ");
			}
			texture = store->createTexture(name, image);
		}	 
	}

	void SpriteInternal::update(float dt)
	{
	}
}