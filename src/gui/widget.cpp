#include "widget.hpp"

namespace ve
{
	Widget::Widget(Ptr<render::Scene> scene_)
	{
		scene = scene_;
	}

	Ptr<render::Scene> Widget::getScene() const
	{
		return scene;
	}
}