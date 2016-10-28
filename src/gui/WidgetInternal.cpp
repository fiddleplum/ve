#include "widget_internal.h"

namespace ve
{
	WidgetInternal::WidgetInternal(UsePtr<Scene> scene_)
	{
		scene = scene_;
	}

	UsePtr<Scene> WidgetInternal::getScene() const
	{
		return scene;
	}
}