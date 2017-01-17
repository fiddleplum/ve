#pragma once

#include "util/rect.hpp"
#include "render/scene.hpp"

namespace ve
{
	class Widget
	{
	public:
		// Constructor.
		Widget(Ptr<render::Scene> scene);

		// Virtual destructor.
		virtual ~Widget() = default;

		// Internal to gui. Returns the depth.
		virtual float getDepth() const = 0;

		// Internal to gui. Sets the depth. Increments depth for others like panel to set their successive widgets depth correctly.
		virtual void setDepth(float & depth) = 0;

		// Internal to gui. Returns the bounds of the widget.
		virtual Recti getBounds() const = 0;

		// Internal to gui. Sets the bounds of the widget. If the widget is inside a Panel, this will be overwritten in the update.
		virtual void setBounds(Recti bounds) = 0;

		// Internal to gui. Called when the user moves the cursor within the widget or out of the widget.
		virtual void onCursorPositionChanged(std::optional<Vector2i> cursorPosition) = 0;

		// Internal to gui. Updates the widget.
		virtual void update(float dt) = 0;

	protected:
		// Returns the scene used by the widget (and its parent gui).
		Ptr<render::Scene> getScene() const;

	private:
		Ptr<render::Scene> scene;
	};
}