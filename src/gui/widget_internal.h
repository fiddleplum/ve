#pragma once

#include "widget.h"
#include "rect.h"
#include "render/scene.h"

namespace ve
{
	class WidgetInternal : public virtual Widget
	{
	public:
		// Constructor.
		WidgetInternal(UsePtr<Scene> scene);

		// Sets the bounds of the widget.
		virtual void setBounds(Recti bounds) = 0;

		// Updates the widget.
		virtual void update(float dt) = 0;

		// Virtual destructor.
		virtual ~WidgetInternal() = default;

	protected:
		// Returns the scene used by the widget (and its parent gui).
		UsePtr<Scene> getScene() const;

	private:
		UsePtr<Scene> scene;
	};
}