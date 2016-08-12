#include "element.h"

namespace ve
{
	namespace gui
	{
		Recti Element::getBounds() const
		{
			return bounds;
		}

		void Element::setBounds(Recti bounds_)
		{
			bounds = bounds_;
			handleNewBounds();
		}
	}
}

