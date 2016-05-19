#include "element.h"

namespace ve
{
	namespace gui
	{
		bool Element::canHaveFocus()
		{
			return false;
		}

		bool Element::hasFocus() const
		{
			return false;
		}

		void Element::setFocus(bool value)
		{
		}

		bool Element::handleEvent(Event const & event, std::optional<Vector2i> cursorPosition)
		{
			// Mouse clicks and wheel turns are automatically consumed by all elements by default.
			if(event.is(Event::MouseButton))
			{
				auto mbEvent = event.as<MouseButtonEvent>();
				if(mbEvent.pressed && cursorIsOver(cursorPosition))
				{
					return true;
				}
			}
			else if (event.is(Event::MouseWheel))
			{
				if(cursorIsOver(cursorPosition))
				{
					return true;
				}
			}
			return false;
		}

		bool Element::cursorIsOver(std::optional<Vector2i> cursorPosition) const
		{
			return cursorPosition && getBounds().contains(cursorPosition.value());
		}
	}
}

