#pragma once

#include "text_button.h"
#include "widget_internal.h"

namespace ve
{
	class TextButtonInternal : public virtual TextButton, public virtual WidgetInternal
	{
	public:
		// Constructor.
		TextButtonInternal(UsePtr<Scene> scene);

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float depth) override;

		// Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Updates the text button.
		void update(float dt);

		// Virtual destructor.
		~TextButtonInternal();
	};
}