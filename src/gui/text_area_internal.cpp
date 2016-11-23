#include "text_area_internal.h"
#include "app_internal.h"
#include "util/stringutil.h"

namespace ve
{
	TextAreaInternal::TextAreaInternal(Ptr<Scene> scene)
		: WidgetInternal(scene)
	{
	}

	TextAreaInternal::~TextAreaInternal()
	{

	}

	void TextAreaInternal::setFont(std::string const & fontFace, int fontSize)
	{
		auto store = getAppInternal()->getStoreInternal();
		font = store->getFont(fontFace + std::to_string(fontSize));
		if (!font)
		{
			font = store->loadFont(fontFace + std::to_string(fontSize), fontFace, fontSize);
		}
	}

	void TextAreaInternal::setText(std::string const & text)
	{
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
		}

		// Construct the models form the text.
		for (auto const & pair : meshes)
		{
		}
	}

	void TextAreaInternal::setColor(Vector4f color)
	{

	}

	float TextAreaInternal::getDepth() const
	{
		return depth;
	}

	void TextAreaInternal::setDepth(float depth_)
	{
		depth = depth_;
		for (auto & model : models)
		{
			model->setDepth(depth);
		}
	}

	Recti TextAreaInternal::getBounds() const
	{
		return bounds;
	}

	void TextAreaInternal::setBounds(Recti bounds_)
	{
		bounds = bounds;
	}

	void TextAreaInternal::update(float dt)
	{

	}
}