#pragma once

#include "gui/text_button.h"
#include "gui/widget_internal.h"

namespace ve
{
	class TextButtonInternal : public virtual TextButton, public virtual WidgetInternal
	{
	public:
		// Constructor.
		TextButtonInternal(Ptr<Scene> scene);

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