#include "gui/sprite.hpp"
#include "store.hpp"

namespace ve
{
	Sprite::Sprite(Ptr<Scene> scene)
		: Widget(scene)
	{
		mesh.setNew();
		mesh->formatTypes = {Mesh::POSITION_2D, Mesh::UV0};
		mesh->vertices = {0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1};
		mesh->indices = {0, 1, 2, 2, 3, 0};
		vbo.setNew(mesh);

		auto material = store.materials.get("gui");
		originUniformLocation = material->getShader()->getUniformInfo("origin").location;
		texSizeUniformLocation = material->getShader()->getUniformInfo("texSize").location;
		texUniformLocation = material->getShader()->getUniformInfo("tex").location;
		model = scene->createModel();
		model->setVertexBufferObject(vbo);
		model->setMaterial(material);
		model->setUniformsFunction([this](Material const & material)
		{
			material.getUniform(originUniformLocation).as<UniformVector2f>()->value = (Vector2f)bounds.min;
			material.getUniform(texSizeUniformLocation).as<UniformVector2f>()->value = (Vector2f)texture->getSize();
			material.getUniform(texUniformLocation).as<UniformTexture2d>()->texture = texture;
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
		updateVbo();
	}

	Vector2i Sprite::getTextureCoords() const
	{
		return textureCoords;
	}

	void Sprite::setTextureCoords(Vector2i coords)
	{
		textureCoords = coords;
		updateVbo();
	}

	void Sprite::setImage(std::string const & name)
	{
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

	void Sprite::updateVbo()
	{
		mesh->vertices = {
			0, 0, (float)textureCoords[0], (float)textureCoords[1],
			(float)bounds.getSize()[0], 0, (float)textureCoords[0] + bounds.getSize()[0], (float)textureCoords[1],
			(float)bounds.getSize()[0], (float)bounds.getSize()[1], (float)textureCoords[0] + bounds.getSize()[0], (float)textureCoords[1] + bounds.getSize()[1],
			0, (float)bounds.getSize()[1], (float)textureCoords[0], (float)textureCoords[1] + bounds.getSize()[1]
		};
		vbo->updateVertices(mesh);
	}
}