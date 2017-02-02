#include "gui/text_area.hpp"
#include "util/stringutil.hpp"

namespace ve
{
	TextArea::TextArea(Ptr<render::Scene> const & scene, Ptr<render::Shader> const & shader)
		: Widget(scene, shader)
	{
		originUniformLocation = shader->getUniformInfo("origin").location;
		imageSizeUniformLocation = shader->getUniformInfo("imageSize").location;
		imageUniformLocation = shader->getUniformInfo("image").location;
		colorUniformLocation = shader->getUniformInfo("color").location;
	}

	TextArea::~TextArea()
	{
		for (auto model : models)
		{
			getScene()->destroyModel(model);
		}
	}

	void TextArea::setFont(Ptr<render::Font> const & font_)
	{
		font = font_;
		updateModels();
	}

	void TextArea::setText(std::string const & text_)
	{
		text = text_;
		updateModels();
	}

	Vector4f TextArea::getColor() const
	{
		return color;
	}

	void TextArea::setColor(Vector4f color_)
	{
		color = color_;
	}

	float TextArea::getDepth() const
	{
		return depth;
	}

	void TextArea::setDepth(float depth_)
	{
		depth = depth_;
		for (auto & model : models)
		{
			model->setDepth(depth);
		}
	}

	Recti TextArea::getBounds() const
	{
		return bounds;
	}

	void TextArea::setBounds(Recti bounds_)
	{
		bounds = bounds_;
	}

	void TextArea::onCursorPositionChanged(std::optional<Vector2i> cursorPosition)
	{

	}

	void TextArea::update(float dt)
	{

	}

	void TextArea::updateModels()
	{
		for (auto & model : models)
		{
			getScene()->destroyModel(model);
		}
		models.clear();

		// A text area may use multiple textures for different code point areas. A separate model is created for each texture.
		std::map<Ptr<render::Image>, std::vector<float>> meshVertices;
		std::map<Ptr<render::Image>, std::vector<unsigned int>> meshIndices;
		Vector2i cursor {0, font->getLineHeight()};
		for (size_t i = 0; i < text.size();)
		{
			// Get the next character, move i forward.
			unsigned int c = getChar(text, i);

			if (c == '\n')
			{
				cursor[1] += font->getLineHeight();
				cursor[0] = 0;
				continue;
			}
			else if (c == '\r')
			{
				continue;
			}
			else if (c == '\t')
			{
				cursor[0] += font->getLineHeight();
				continue;
			}

			// Get the font information for the next character.
			render::Font::GlyphCoords const & glyphCoords = font->getGlyphCoordsFromChar(c);
			Ptr<render::Image> glyphImage = font->getImageFromChar(c);

			// Setup the vertice and indices for this glyph.
			Rectf glyphBounds;
			glyphBounds.min = Vector2f(cursor + glyphCoords.offset);
			glyphBounds.setSize(Vector2f(glyphCoords.uvBounds.getSize()));
			Rectf glyphUVBounds = Rectf(glyphCoords.uvBounds);
			glyphUVBounds.max += Vector2f {1, 1};
			std::vector<float> & vertices = meshVertices[glyphImage];
			int startingIndex = (int)vertices.size() / 4;
			vertices.push_back(glyphBounds.min[0]);
			vertices.push_back(glyphBounds.min[1]);
			vertices.push_back(glyphUVBounds.min[0]);
			vertices.push_back(glyphUVBounds.min[1]);
			vertices.push_back(glyphBounds.max[0]);
			vertices.push_back(glyphBounds.min[1]);
			vertices.push_back(glyphUVBounds.max[0]);
			vertices.push_back(glyphUVBounds.min[1]);
			vertices.push_back(glyphBounds.max[0]);
			vertices.push_back(glyphBounds.max[1]);
			vertices.push_back(glyphUVBounds.max[0]);
			vertices.push_back(glyphUVBounds.max[1]);
			vertices.push_back(glyphBounds.min[0]);
			vertices.push_back(glyphBounds.max[1]);
			vertices.push_back(glyphUVBounds.min[0]);
			vertices.push_back(glyphUVBounds.max[1]);
			std::vector<unsigned int> & indices = meshIndices[glyphImage];
			indices.push_back(startingIndex + 0);
			indices.push_back(startingIndex + 1);
			indices.push_back(startingIndex + 2);
			indices.push_back(startingIndex + 2);
			indices.push_back(startingIndex + 3);
			indices.push_back(startingIndex + 0);
			cursor[0] += glyphCoords.advance;
		}

		// Construct the models form the text.
		for (auto const & pair : meshVertices)
		{
			auto image = pair.first;
			auto model = getScene()->createModel();
			auto mesh = OwnPtr<render::Mesh>::returnNew();
			meshes.push_back(mesh);
			mesh->setVertices(meshVertices[image]);
			mesh->setIndices(meshIndices[image]);
			mesh->setVertexFormat({render::Mesh::POSITION_2D, render::Mesh::UV0});
			model->setMesh(mesh);
			model->setShader(getShader());
			model->setImageAtSlot(image, 0);
			model->setUniformsFunction([this, image](Ptr<render::Shader> const & shader)
			{
				shader->setUniformValue<Vector2f>(originUniformLocation, (Vector2f)bounds.min);
				shader->setUniformValue<Vector2f>(imageSizeUniformLocation, (Vector2f)image->getSize());
				shader->setUniformValue<int>(imageUniformLocation, 0);
				shader->setUniformValue<Vector4f>(colorUniformLocation, color);
			});
			model->setDepth(depth);
			models.push_back(model);
		}
	}
}