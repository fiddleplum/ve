#pragma once

#include "ptr.h"
#include "panel.h"

namespace ve
{
	class Gui
	{
	public:
		// Returns the root panel used by the gui.
		virtual UsePtr<Panel> getRootPanel() const = 0;

	protected:
		virtual ~Gui() = default;
	};
}