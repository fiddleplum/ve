#include "text_area.hpp"
#include "store.hpp"
#include "util/stringutil.hpp"

namespace ve
{
	TextArea::TextArea(Ptr<Scene> scene)
		: Widget(scene)
	{
		material = store.materials.get("gui");
		originUniformLocation = material->getShader()->getUniformInfo("origin").location;
		texSizeUniformLocation = material->getShader()->getUniformInfo("texSize").location;
		texUniformLocation = material->getShader()->getUniformInfo("tex").location;
		colorUniformLocation = material->getShader()->getUniformInfo("color").location;
	}

	TextArea::~TextArea()
	{

	}

	void TextArea::setFont(std::string const & fontFace, int fontSize)
	{
		font = store.fonts.get(fontFace + std::to_string(fontSize));
		if (!font)
		{
			font = store.fonts.create(fontFace + std::to_string(fontSize), fontFace, fontSize);
		}
	}

	void TextArea::setText(std::string const & text)
	{
		models.clear();
		vbos.clear();

		std::map<Ptr<Texture>, Mesh> meshes;
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
			Font::GlyphCoords const & glyphCoords = font->getGlyphCoordsFromChar(c);
			Ptr<Texture> glyphTexture = font->getTextureFromChar(c);

			// Find which model to append, or create new model.
			auto it = meshes.find(glyphTexture);
			if (it == meshes.end())
			{
				meshes.insert({glyphTexture, Mesh()});
				it = meshes.find(glyphTexture);
				it->second.formatTypes = {Mesh::POSITION_2D, Mesh::UV0};
			}
			Mesh & mesh = it->second;
			Rectf glyphBounds;
			glyphBounds.min = Vector2f(cursor + glyphCoords.offset);
			glyphBounds.setSize(Vector2f(glyphCoords.uvBounds.getSize()));
			Rectf glyphUVBounds = Rectf(glyphCoords.uvBounds);
			glyphUVBounds.max += Vector2f {1, 1};
			int startingIndex = (int)mesh.vertices.size() / 4;
			mesh.vertices.push_back(glyphBounds.min[0]);
			mesh.vertices.push_back(glyphBounds.min[1]);
			mesh.vertices.push_back(glyphUVBounds.min[0]);
			mesh.vertices.push_back(glyphUVBounds.min[1]);
			mesh.vertices.push_back(glyphBounds.max[0]);
			mesh.vertices.push_back(glyphBounds.min[1]);
			mesh.vertices.push_back(glyphUVBounds.max[0]);
			mesh.vertices.push_back(glyphUVBounds.min[1]);
			mesh.vertices.push_back(glyphBounds.max[0]);
			mesh.vertices.push_back(glyphBounds.max[1]);
			mesh.vertices.push_back(glyphUVBounds.max[0]);
			mesh.vertices.push_back(glyphUVBounds.max[1]);
			mesh.vertices.push_back(glyphBounds.min[0]);
			mesh.vertices.push_back(glyphBounds.max[1]);
			mesh.vertices.push_back(glyphUVBounds.min[0]);
			mesh.vertices.push_back(glyphUVBounds.max[1]);
			mesh.indices.push_back(startingIndex + 0);
			mesh.indices.push_back(startingIndex + 1);
			mesh.indices.push_back(startingIndex + 2);
			mesh.indices.push_back(startingIndex + 2);
			mesh.indices.push_back(startingIndex + 3);
			mesh.indices.push_back(startingIndex + 0);
			cursor[0] += glyphCoords.advance;
		}

		// Construct the models form the text.
		for (auto const & pair : meshes)
		{
			auto texture = pair.first;
			auto model = getScene()->createModel();
			auto vbo = OwnPtr<VertexBufferObject>::returnNew(pair.second);
			vbos.push_back(vbo);
			model->setVertexBufferObject(vbo);
			model->setMaterial(material);
			model->setUniformsFunction([this, texture](Material const & material)
			{
				material.getUniform(originUniformLocation).as<UniformVector2f>()->value = (Vector2f)bounds.min;
				material.getUniform(texSizeUniformLocation).as<UniformVector2f>()->value = (Vector2f)texture->getSize();
				material.getUniform(texUniformLocation).as<UniformTexture2d>()->texture = texture;
				material.getUniform(colorUniformLocation).as<UniformVector4f>()->value = color;
			});
			models.push_back(model);
		}
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

	void TextArea::update(float dt)
	{

	}
}