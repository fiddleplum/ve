#pragma once

#include "util/rect.h"

namespace ve
{
	class Widget
	{
	public:
		virtual Recti getBounds() const = 0;

	protected:
		// Virtual destructor.
		virtual ~Widget() = default;
	};
}