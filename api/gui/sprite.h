#pragma once

#include "widget.h"
#include <string>

namespace ve
{
	class Sprite : public virtual Widget
	{
	public:
		// Loads an image from the store into the sprite.
		virtual void setImage(std::string const & name) = 0;

	protected:
		// Virtual destructor.
		virtual ~Sprite() = default;
	};
}