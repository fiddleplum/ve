#include "gui.hpp"
#include "store.hpp"

namespace ve
{
	Gui::Gui()
	{
		createResources();

		scene.setNew();
		scene->setUniformsFunction([this](Ptr<Shader> const & shader)
		{
			Recti bounds = this->getRootPanel()->getBounds();
			shader->setUniformValue<Vector2f>("guiSize", (Vector2f)(bounds.max - bounds.min + Vector2i {1, 1}));
		});
		root.setNew(scene);
		float depth = 0;
		root->setDepth(depth);
	}

	Gui::~Gui()
	{
		root.setNull();
		scene.setNull();
	}

	Ptr<Panel> Gui::getRootPanel() const
	{
		return root;
	}

	Ptr<Scene> Gui::getScene() const
	{
		return scene;
	}

	void Gui::setSize(Vector2i size)
	{
		root->setBounds({{0, 0}, {size[0] - 1, size[1] - 1}});
	}

	void Gui::update(float dt)
	{
		root->update(dt);
	}

	void Gui::createResources()
	{
		auto shader = store.shaders.get("gui");
		if (!shader)
		{
			Config shaderConfig;
			shaderConfig.children["vertex"].text =
				"#version 400\n"
				"uniform vec2 origin;\n"
				"uniform vec2 texSize;\n"
				"uniform vec2 guiSize;\n"
				"attribute vec2 position2d;\n"
				"attribute vec2 uv0;\n"
				"varying vec2 v_uv0;\n"
				"void main(void) {\n"
				"	gl_Position = vec4(2 * (origin.x + position2d.x) / guiSize.x - 1, -2 * (origin.y + position2d.y) / guiSize.y + 1, 0, 1);\n"
				"	v_uv0 = vec2(uv0.x / texSize.x, uv0.y / texSize.y);\n"
				"}\n";
			shaderConfig.children["fragment"].text =
				"#version 400\n"
				"varying vec2 v_uv0;\n"
				"uniform vec4 color;\n"
				"uniform sampler2D tex;\n"
				"void main(void) {\n"
				"	gl_FragColor = color * texture(tex, clamp(v_uv0, 0, 1));\n"
				"}\n";
			shaderConfig.children["blending"].text = "alpha";
			shader = store.shaders.create("gui", shaderConfig);
		}
	}
}