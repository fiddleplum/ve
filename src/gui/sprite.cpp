#include "gui/sprite.hpp"
#include "store.hpp"

namespace ve
{
	Sprite::Sprite(Ptr<render::Scene> scene)
		: Widget(scene)
	{
		Mesh mesh;
		mesh.formatTypes = {Mesh::POSITION_2D, Mesh::UV0};
		mesh.vertices = {0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1};
		mesh.indices = {0, 1, 2, 2, 3, 0};
		vbo.setNew(mesh);

		auto shader = store.shaders.get("gui");
		originUniformLocation = shader->getUniformInfo("origin").location;
		texSizeUniformLocation = shader->getUniformInfo("texSize").location;
		texUniformLocation = shader->getUniformInfo("tex").location;
		colorUniformLocation = shader->getUniformInfo("color").location;

		model = scene->createModel();
		model->setVertexBufferObject(vbo);
		model->setShader(shader);
		model->setTextureAtSlot(texture, 0);
		model->setUniformsFunction([this](Ptr<render::Shader> const & shader)
		{
			if (texture.isValid())
			{
				shader->setUniformValue<Vector2f>(originUniformLocation, (Vector2f)bounds.min);
				shader->setUniformValue<Vector2f>(texSizeUniformLocation, (Vector2f)texture->getSize());
				shader->setUniformValue<int>(texUniformLocation, 0);
				shader->setUniformValue<Vector4f>(colorUniformLocation, Vector4f::filled(1));
			}
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

	void Sprite::setDepth(float & depth)
	{
		model->setDepth(depth);
		depth++;
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
		auto texture = store.textures.get(name);
		if (!texture)
		{
			auto image = store.images.get(name);
			if (!image)
			{
				throw std::runtime_error("Image name '" + name + "' not found. ");
			}
			texture = store.textures.create(name, image);
		}
		setTexture(texture);
	}

	void Sprite::setTexture(std::string const & name)
	{
		auto texture = store.textures.get(name);
		if (!texture)
		{
			throw std::runtime_error("Texture name '" + name + "' not found. ");
		}
		setTexture(texture);
	}

	void Sprite::setTexture(Ptr<render::Texture> texture_)
	{
		texture = texture_;
		model->setTextureAtSlot(texture, 0);
	}

	void Sprite::update(float dt)
	{
	}

	void Sprite::updateVbo()
	{
		std::vector<float> vertices = {
			0, 0, (float)textureCoords[0], (float)textureCoords[1],
			(float)bounds.getSize()[0], 0, (float)textureCoords[0] + bounds.getSize()[0], (float)textureCoords[1],
			(float)bounds.getSize()[0], (float)bounds.getSize()[1], (float)textureCoords[0] + bounds.getSize()[0], (float)textureCoords[1] + bounds.getSize()[1],
			0, (float)bounds.getSize()[1], (float)textureCoords[0], (float)textureCoords[1] + bounds.getSize()[1]
		};
		vbo->updateVertices(vertices);
	}
}