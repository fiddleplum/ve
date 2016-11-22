#pragma once

#include "util/ptr.h"
#include "util/vector.h"
#include "gui/sprite.h"
#include "gui/text_area.h"
#include "gui/text_button.h"

namespace ve
{
	class Panel : public virtual Widget
	{
	public:
		// Creates a sprite contained in the panel.
		virtual Ptr<Sprite> createSprite() = 0;

		// Creates a text area contained in the panel.
		virtual Ptr<TextArea> createTextArea() = 0;

		// Creates a text button contained in the panel.
		virtual Ptr<TextButton> createTextButton() = 0;

		// Destroys a widget.
		virtual void destroyWidget(Ptr<Widget> widget) = 0;

		// Sets the relative bounds for the widget.
		virtual void setWidgetBounds(Ptr<Widget> widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset) = 0;

	protected:
		// Virtual destructor.
		virtual ~Panel() = default;
	};
}