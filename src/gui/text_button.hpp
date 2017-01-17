#pragma once

#include "gui/widget.hpp"
#include "util/ptr.hpp"

namespace ve
{
	class TextButton : public Widget
	{
	public:
		// Constructor.
		TextButton(Ptr<render::Scene> scene);

		// Internal to gui. Returns the depth.
		float getDepth() const override;

		// Internal to gui. Sets the depth.
		void setDepth(float & depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Internal to gui. Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Internal to gui. Called when the user moves the cursor within the widget or out of the widget.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition) override;

		// Internal to gui. Updates the text button.
		void update(float dt) override;

		// Virtual destructor.
		~TextButton();
	};
}