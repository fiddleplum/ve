#include "widget.hpp"

namespace ve
{
	Widget::Widget(Ptr<Scene> scene_)
	{
		scene = scene_;
	}

	Ptr<Scene> Widget::getScene() const
	{
		return scene;
	}
}