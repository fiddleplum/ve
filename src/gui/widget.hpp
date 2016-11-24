#pragma once

#include "util/rect.hpp"
#include "render/scene.hpp"

namespace ve
{
	class Widget
	{
	public:
		// Constructor.
		Widget(Ptr<Scene> scene);

		// Virtual destructor.
		virtual ~Widget() = default;

		// Returns the depth.
		virtual float getDepth() const = 0;

		// Sets the depth.
		virtual void setDepth(float depth) = 0;

		// Returns the bounds of the widget.
		virtual Recti getBounds() const = 0;

		// Sets the bounds of the widget.
		virtual void setBounds(Recti bounds) = 0;

		// Updates the widget.
		virtual void update(float dt) = 0;

	protected:
		// Returns the scene used by the widget (and its parent gui).
		Ptr<Scene> getScene() const;

	private:
		Ptr<Scene> scene;
	};
}