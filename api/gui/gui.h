#pragma once

#include "util/ptr.h"
#include "gui/panel.h"

namespace ve
{
	class Gui
	{
	public:
		// Returns the root panel used by the gui.
		virtual Ptr<Panel> getRootPanel() const = 0;

	protected:
		virtual ~Gui() = default;
	};
}