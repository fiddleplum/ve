#include "gui/sprite.hpp"

namespace ve
{
	Sprite::Sprite(Ptr<render::Scene> const & scene, Ptr<render::Shader> const & shader)
		: Widget(scene, shader)
	{
		mesh.setNew();
		mesh->setVertices(0, {0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1}, sizeof(float) * 4);
		mesh->setVertexComponent(0, 2, 0, 0, false);
		mesh->setVertexComponent(1, 2, sizeof(float) * 2, 0, false);
		mesh->setIndices({0, 2, 1, 0, 3, 2});

		originUniformLocation = shader->getUniformInfo("origin").location;
		imageSizeUniformLocation = shader->getUniformInfo("imageSize").location;
		imageUniformLocation = shader->getUniformInfo("image").location;
		colorUniformLocation = shader->getUniformInfo("color").location;

		model = scene->createModel();
		model->setMesh(mesh);
		model->setShader(shader);
		model->setImageAtSlot(image, 0);
		model->setUniformsFunction([this](Ptr<render::Shader> const & shader)
		{
			if (image.isValid())
			{
				shader->setUniformValue<Vector2f>(originUniformLocation, (Vector2f)bounds.min);
				shader->setUniformValue<Vector2f>(imageSizeUniformLocation, (Vector2f)image->getSize());
				shader->setUniformValue<int>(imageUniformLocation, 0);
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
		updateMesh();
	}

	Vector2i Sprite::getImageOffset() const
	{
		return imageOffset;
	}

	void Sprite::setImageOffset(Vector2i offset)
	{
		imageOffset = offset;
		updateMesh();
	}

	void Sprite::setImage(Ptr<render::Image> const & image_)
	{
		image = image_;
		model->setImageAtSlot(image, 0);
	}

	void Sprite::onCursorPositionChanged(std::optional<Vector2i> cursorPosition)
	{
	}

	void Sprite::update(float dt)
	{
	}

	void Sprite::updateMesh()
	{
		std::vector<float> vertices = {
			0, 0, (float)imageOffset[0], (float)imageOffset[1],
			(float)bounds.getSize()[0], 0, (float)imageOffset[0] + bounds.getSize()[0], (float)imageOffset[1],
			(float)bounds.getSize()[0], (float)bounds.getSize()[1], (float)imageOffset[0] + bounds.getSize()[0], (float)imageOffset[1] + bounds.getSize()[1],
			0, (float)bounds.getSize()[1], (float)imageOffset[0], (float)imageOffset[1] + bounds.getSize()[1]
		};
		mesh->setVertices(0, vertices, sizeof(float) * 4);
	}
}