#pragma once

#include "gui/widget.hpp"
#include "render/font.hpp"

namespace ve
{
	class TextArea : public Widget
	{
	public:
		// Constructor.
		TextArea(Ptr<render::Scene> scene);

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

		// Internal to gui. Returns the depth.
		float getDepth() const override;

		// Internal to gui. Sets the depth.
		void setDepth(float & depth) override;

		// Returns the bounds of the widget.
		Recti getBounds() const override;

		// Internal to gui. Sets the bounds of the widget.
		void setBounds(Recti bounds) override;

		// Internal to gui. Called when the user moves the cursor within the widget or out of the widget.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition) override;

		// Internal to gui. Updates the widget.
		void update(float dt) override;

	private:
		Recti bounds;
		float depth;
		Vector4f color;
		std::vector<OwnPtr<render::VertexBufferObject>> vbos;
		std::vector<Ptr<render::Model>> models;
		Ptr<render::Font> font;
		Ptr<render::Shader> shader;
		unsigned int originUniformLocation;
		unsigned int texSizeUniformLocation;
		unsigned int texUniformLocation;
		unsigned int colorUniformLocation;
	};
}