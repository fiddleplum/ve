#pragma once

#include "ptr.h"
#include "vector.h"
#include "sprite.h"
#include "text_button.h"

namespace ve
{
	class Panel : public virtual Widget
	{
	public:
		// Creates a sprite contained in the panel.
		virtual UsePtr<Sprite> createSprite() = 0;

		// Creates a text button contained in the panel.
		virtual UsePtr<TextButton> createTextButton() = 0;

		// Destroys a widget.
		virtual void destroyWidget(UsePtr<Widget> widget) = 0;

		// Sets the relative bounds for the widget.
		virtual void setBounds(UsePtr<Widget> widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset) = 0;

	protected:
		// Virtual destructor.
		virtual ~Panel() = default;
	};
}