#pragma once

#include "gui/text_area.h"
#include "gui/widget_internal.h"
#include "util/font.h"

namespace ve
{
	class TextAreaInternal : public virtual TextArea, public virtual WidgetInternal
	{
	public:
		// Constructor.
		TextAreaInternal(Ptr<Scene> scene);

		// Destructor.
		~TextAreaInternal();

		// Sets the font.
		void setFont(std::string const & fontFace, int fontSize) override;

		// Sets the text.
		void setText(std::string const & text) override;

		// Sets the color.
		void setColor(Vector4f color) override;

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
		std::vector<Ptr<Model>> models;
		Ptr<Font> font;
	};
}