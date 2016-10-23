#pragma once

#include "widget.h"

namespace ve
{
	class WidgetInternal : public virtual Widget
	{
	public:
		virtual void update(float dt) = 0;

		// Virtual destructor.
		virtual ~WidgetInternal() = default;
	};
}