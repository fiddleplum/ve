#pragma once

#include "widget.h"
#include <string>

namespace ve
{
	class TextArea : public virtual Widget
	{
	public:
		// Sets the font.
		virtual void setFont(std::string const & fontName) = 0;

		// Sets the font size.
		virtual void setFontSize(int fontSize) = 0;

		// Sets the text.
		virtual void setText(std::string const & text) = 0;

		// Sets the color.
		virtual void setColor(Vector4f color) = 0;

	protected:
		// Virtual destructor.
		virtual ~TextArea() = default;
	};
}