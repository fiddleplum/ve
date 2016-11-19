#include "widget_internal.h"

namespace ve
{
	WidgetInternal::WidgetInternal(Ptr<Scene> scene_)
	{
		scene = scene_;
	}

	Ptr<Scene> WidgetInternal::getScene() const
	{
		return scene;
	}
}