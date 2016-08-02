#include "viewport.h"

namespace ve
{
	namespace gui
	{
		Recti Viewport::getBounds() const
		{
			return bounds;
		}

		void Viewport::setPosition(Vector2i position)
		{
			bounds.setPosition(position);
		}

		void Viewport::setSize(Vector2i size)
		{
			bounds.setSize(size);
		}

		void Viewport::render(Vector2i windowSize) const
		{
		}
	}
}

