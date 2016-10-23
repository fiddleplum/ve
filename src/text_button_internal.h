#pragma once

#include "text_button.h"
#include "widget_internal.h"

namespace ve
{
	class TextButtonInternal : public virtual TextButton, public virtual WidgetInternal
	{
	public:
		// Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		void update(float dt);

		// Virtual destructor.
		~TextButtonInternal();
	};
}