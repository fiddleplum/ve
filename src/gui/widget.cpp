#include "widget.hpp"

namespace ve
{
	Widget::Widget(Ptr<render::Scene> const & scene_, Ptr<render::Shader> const & shader_)
	{
		scene = scene_;
		shader = shader_;
	}

	Ptr<render::Scene> Widget::getScene() const
	{
		return scene;
	}

	Ptr<render::Shader> Widget::getShader() const
	{
		return shader;
	}
}