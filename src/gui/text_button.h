#pragma once

#include "gui/widget.h"
#include "util/ptr.h"

namespace ve
{
	class TextButton : public Widget
	{
	public:
		// Constructor.
		TextButton(Ptr<Scene> scene);

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Updates the text button.
		void update(float dt);

		// Virtual destructor.
		~TextButton();
	};
}