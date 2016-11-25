#pragma once

#include "gui/widget.hpp"
#include "render/font.hpp"

namespace ve
{
	class TextArea : public Widget
	{
	public:
		// Constructor.
		TextArea(Ptr<Scene> scene);

		// Destructor.
		~TextArea();

		// Sets the font.
		void setFont(std::string const & fontFace, int fontSize);

		// Sets the text.
		void setText(std::string const & text);

		// Returns the color.
		Vector4f getColor() const;

		// Sets the color.
		void setColor(Vector4f color);

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float depth) override;

		// Returns the bounds of the widget.
		Recti getBounds() const override;

		// Sets the bounds of the widget.
		void setBounds(Recti bounds) override;

		// Updates the widget.
		void update(float dt) override;

	private:
		Recti bounds;
		float depth;
		Vector4f color;
		std::vector<OwnPtr<VertexBufferObject>> vbos;
		std::vector<Ptr<Model>> models;
		Ptr<Font> font;
		Ptr<Material> material;
		unsigned int originUniformLocation;
		unsigned int texSizeUniformLocation;
		unsigned int texUniformLocation;
		unsigned int colorUniformLocation;
	};
}