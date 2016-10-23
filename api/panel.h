#pragma once

#include "ptr.h"
#include "coord.h"
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

		// Sets the relative bounds for the widget.
		virtual void setBounds(UsePtr<Widget> widget, Coord2f originInPanel, Coord2f originInWidget, Coord2i originOffset, Coord2f sizeInPanel, Coord2i sizeOffset) = 0;

	protected:
		// Virtual destructor.
		virtual ~Panel() = default;
	};
}