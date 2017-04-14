#include "gui/gui.hpp"

namespace ve
{
	Gui::Gui(Ptr<render::Render> const & render)
	{
		Config shaderConfig;
		shaderConfig.children["vertex"].text =
			"#version 430\n"
			"uniform vec2 origin;\n"
			"uniform vec2 imageSize;\n"
			"uniform vec2 guiSize;\n"
			"uniform float flipY;\n"
			"attribute vec2 position2d;\n"
			"attribute vec2 uv0;\n"
			"varying vec2 v_uv0;\n"
			"void main(void) {\n"
			"	gl_Position = vec4(2 * (origin.x + position2d.x) / guiSize.x - 1, flipY * (-2 * (origin.y + position2d.y) / guiSize.y + 1), 0, 1);\n"
			"	v_uv0 = vec2(uv0.x / imageSize.x, uv0.y / imageSize.y);\n"
			"}\n";
		shaderConfig.children["fragment"].text =
			"#version 430\n"
			"varying vec2 v_uv0;\n"
			"uniform vec4 color;\n"
			"uniform sampler2D image;\n"
			"void main(void) {\n"
			"	gl_FragColor = color * texture(image, clamp(v_uv0, 0, 1));\n"
			"}\n";
		shaderConfig.children["blending"].text = "alpha";
		shader = render->shaders.create(shaderConfig);

		scene = render->scenes.create();
		scene->setUniformsFunction([this](Ptr<render::Shader> const & shader)
		{
			Recti bounds = this->getRootPanel()->getBounds();
			shader->setUniformValue<Vector2f>("guiSize", (Vector2f)(bounds.max - bounds.min + Vector2i {1, 1}));
		});
		root.setNew(scene, shader);
		float depth = 0;
		root->setDepth(depth);
	}

	Gui::~Gui()
	{
	}

	Ptr<Panel> Gui::getRootPanel() const
	{
		return root;
	}

	Ptr<render::Scene> Gui::getScene() const
	{
		return scene;
	}

	void Gui::setSize(Vector2i size)
	{
		root->setBounds({{0, 0}, {size[0] - 1, size[1] - 1}});
	}

	void Gui::onCursorPositionChanged(std::optional<Vector2i> cursorPosition)
	{
		root->onCursorPositionChanged(cursorPosition);
	}

	void Gui::update(float dt)
	{
		root->update(dt);
	}
}