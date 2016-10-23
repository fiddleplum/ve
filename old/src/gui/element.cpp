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

		void Element::handleNewBounds()
		{
		}

		float Element::getDepth() const
		{
			return depth;
		}

		void Element::setDepth(float depth_)
		{
			depth = depth_;
		}

		void Element::handleNewDepth()
		{
		}

		bool Element::handleInputEvent(InputEvent const & event, std::optional<Vector2i> cursorPosition)
		{
			return false;
		}

		void Element::update(float dt)
		{
		}

		void Element::preRender(UsePtr<render::Stage> stage)
		{
		}

		bool Element::cursorIsOver(std::optional<Vector2i> cursorPosition) const
		{
			return cursorPosition && bounds.contains(cursorPosition.value());
		}
	}
}

