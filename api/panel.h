#pragma once

#include "ptr.h"
#include "sprite.h"
#include "text_button.h"

namespace ve
{
	class Panel : public virtual Widget
	{
	public:
		virtual UsePtr<Sprite> createSprite() = 0;

		virtual UsePtr<TextButton> createTextButton() = 0;

		// Virtual destructor.
		virtual ~Panel() = default;
	};
}