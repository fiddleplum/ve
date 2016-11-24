#include "gui.hpp"
#include "store.hpp"

namespace ve
{
	Gui::Gui()
	{
		createResources();

		scene.setNew();
		scene->setUniformsFunction([this](Material const & material)
		{
			Recti bounds = this->getRootPanel()->getBounds();
			material.getUniform("guiSize").as<UniformVector2i>()->value = bounds.max - bounds.min + Vector2i {1, 1};
		});
		root.setNew(scene);
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

	void Gui::handleResizeEvent(Vector2i size)
	{
		root->setBounds({{0, 0}, {size[0] - 1, size[1] - 1}});
	}

	void Gui::update(float dt)
	{
		root->update(dt);
	}

	void Gui::createResources()
	{
		auto shader = store.shaders.get("guiShader");
		if (!shader)
		{
			Config shaderConfig;
			shaderConfig.children["vertex"].text =
				"#version 400\n"
				"uniform ivec2 min;\n"
				"uniform ivec2 max;\n"
				"uniform ivec2 guiSize;\n"
				"attribute vec2 position2d;\n"
				"attribute vec2 uv0;\n"
				"varying vec2 v_uv0;\n"
				"void main(void) {\n"
				"   vec2 pos = 2.0 * (min + position2d * (max - min + 1)) / guiSize + vec2(-1, -1);\n"
				"	gl_Position = vec4(pos.x, -pos.y, 0, 1);\n"
				"	v_uv0 = uv0;\n"
				"}\n";
			shaderConfig.children["fragment"].text =
				"#version 400\n"
				"varying vec2 v_uv0;\n"
				"uniform sampler2D tex;\n"
				"void main(void) {\n"
				"	gl_FragColor = texture(tex, clamp(v_uv0, 0, 1));\n"
				"}\n";
			shader = store.shaders.create("guiShader", shaderConfig);
		}
		auto material = store.materials.get("guiMaterial");
		if (!material)
		{
			material = store.materials.create("guiMaterial");
			material->setShader(shader);
		}

		auto vbo = store.vertexBufferObjects.get("guiUnitSquare");
		if (!vbo)
		{
			auto mesh = store.meshes.get("guiUnitSquare");
			if (!mesh)
			{
				mesh = store.meshes.create("guiUnitSquare");
				mesh->formatTypes = {Mesh::POSITION_2D, Mesh::UV0};
				mesh->vertices = {0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1};
				mesh->indices = {0, 1, 2, 2, 3, 0};
			}
			vbo = store.vertexBufferObjects.create("guiUnitSquare", mesh);
		}
	}
}