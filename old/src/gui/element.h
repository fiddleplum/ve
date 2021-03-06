#pragma once

#include "../event.h"
#include "../rect.h"
#include "../ptr.h"
#include "../render/stage.h"
#include "../std/optional.hpp" // eventually when msvc supports c++17, include <optional>

namespace ve
{
	namespace gui
	{
		// A base class for all elements within a Gui.
		class Element
		{
		protected:
			// Virtual destructor, so it can be deleted properly.
			virtual ~Element() {}

			// Returns the bounds of the element.
			Recti getBounds() const;

			// Sets the bounds of the element of the element. Note that some elements will not change their size to the size, but may be smaller.
			void setBounds(Recti bounds);

			// Called when the bounds have changed. Default does nothing.
			virtual void handleNewBounds();

			// Returns the depth of the element.
			float getDepth() const;

			// Sets the depth value of the element. Used in sorting for rendering.
			void setDepth(float depth);

			// Called when the depth has changed. Default does nothing.
			virtual void handleNewDepth();

			// Called by GuiContainer to handle an event. The cursorPosition is relative to the parent window, and may not be valid. Returns true if the event is consumed. Default does nothing and returns false.
			virtual bool handleInputEvent(InputEvent const & event, std::optional<Vector2i> cursorPosition);

			// Called by Container or Window to update the element once every frame. Default does nothing.
			virtual void update(float dt);

			// Called by Container or Window to update the element after a regular update but before the render. Good for things that keep track of scene elements. Default does nothing.
			virtual void preRender(UsePtr<render::Stage> stage);

			// Returns true if the cursor is over the element. Defaults to just using the bounds.
			virtual bool cursorIsOver(std::optional<Vector2i> cursorPosition) const;

		private:
			Recti bounds = {{0, 0}, {0, 0}};
			float depth = 0;

			friend class Container;
		};
	}
}

/*
Note: Because of Viewport, I really need glScissor to be implemented in the gui container. The reason is that
the viewport can't just intersect bounds and clipBounds, or the view becomes squished, and it would be a pain
to offset and squish the camera center to work with the clipped bounds.

If gui container implements glscissor, the render part becomes much simpler. the event handling changes.
I need to put in the handleevent call if the cursor is valid or not. This goes back to what to pass
into the handleEvent function. Perhaps I need to revisit this whole issue _again_.

I like the idea of consuming the event still, because it greatly reduces the number of calls to
handleEvent on each gui element. Perhaps a small structure of the position and validity should
still be passed in, instead of the whole cursor.

Let's see: An element needs the validity and position, and the ability to change the cursor, and really
nothing else.

Let's use this:
void handleEvent(Event event, bool cursorPositionisValid, Ptr<Cursor> cursor);

Then in Sprite and even in Element, we can remove the clipBounds entirely. For button, it just needs to
check include cursorPositionIsValid in its logic.

For Element not to require clipBounds, Container should not need to know about the bounds of its parents.
The way to do this is to implement glScissorPush and pop so that each push intersects with the previous
on the scissor stack to product the new scissor. For container event handling, it still just is passed
cursorPositionIsValid, and uses its own bounds to update the variable.

This should work.

*/