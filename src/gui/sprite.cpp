#include "gui/sprite.hpp"

namespace ve
{
	Sprite::Sprite(Ptr<render::Scene> const & scene, Ptr<render::Shader> const & shader)
		: Widget(scene, shader)
	{
		Mesh mesh;
		mesh.formatTypes = {Mesh::POSITION_2D, Mesh::UV0};
		mesh.vertices = {0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1};
		mesh.indices = {0, 1, 2, 2, 3, 0};
		vbo.setNew(mesh);

		originUniformLocation = shader->getUniformInfo("origin").location;
		imageSizeUniformLocation = shader->getUniformInfo("imageSize").location;
		imageUniformLocation = shader->getUniformInfo("image").location;
		colorUniformLocation = shader->getUniformInfo("color").location;

		model = scene->createModel();
		model->setVertexBufferObject(vbo);
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
		updateVbo();
	}

	Vector2i Sprite::getImageOffset() const
	{
		return imageOffset;
	}

	void Sprite::setImageOffset(Vector2i offset)
	{
		imageOffset = offset;
		updateVbo();
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

	void Sprite::updateVbo()
	{
		std::vector<float> vertices = {
			0, 0, (float)imageOffset[0], (float)imageOffset[1],
			(float)bounds.getSize()[0], 0, (float)imageOffset[0] + bounds.getSize()[0], (float)imageOffset[1],
			(float)bounds.getSize()[0], (float)bounds.getSize()[1], (float)imageOffset[0] + bounds.getSize()[0], (float)imageOffset[1] + bounds.getSize()[1],
			0, (float)bounds.getSize()[1], (float)imageOffset[0], (float)imageOffset[1] + bounds.getSize()[1]
		};
		vbo->updateVertices(vertices);
	}
}