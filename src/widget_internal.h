#pragma once

#include "widget.h"
#include "rect.h"

namespace ve
{
	class WidgetInternal : public virtual Widget
	{
	public:
		// Sets the bounds of the widget.
		virtual void setBounds(Recti bounds) = 0;

		// Updates the widget.
		virtual void update(float dt) = 0;

		// Virtual destructor.
		virtual ~WidgetInternal() = default;
	};
}