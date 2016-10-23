#pragma once

#include "widget.h"

namespace ve
{
	class TextButton : public virtual Widget
	{
	protected:
		// Virtual destructor.
		virtual ~TextButton() = default;
	};
}