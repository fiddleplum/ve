#include "gui/sprite.h"
#include "store.hpp"

namespace ve
{
	Sprite::Sprite(Ptr<Scene> scene)
		: Widget(scene)
	{
		std::string modelName = "guiUnitSquare";
		auto vbo = store.vertexBufferObjects.get(modelName);
		if (!vbo)
		{
			auto mesh = store.meshes.get(modelName);
			if (!mesh)
			{
				mesh = store.meshes.create(modelName);
				mesh->formatTypes = {Mesh::POSITION_2D, Mesh::UV0};
				mesh->vertices = {0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1};
				mesh->indices = {0, 1, 2, 3, 2, 0};
			}
			vbo = store.vertexBufferObjects.create(modelName, mesh);
		}
		auto material = store.materials.get("guiMaterial");
		if (!material)
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
			material = store.materials.create("guiMaterial");
			material->setShader(shader);
			minUniformLocation = material->getUniform("min")->getLocation();
			maxUniformLocation = material->getUniform("max")->getLocation();
		}
		model = scene->createModel();
		model->setVertexBufferObject(vbo);
		model->setMaterial(material);
		model->setUniformsFunction([this](Material const & material)
		{
			material.getUniform(minUniformLocation).as<UniformVector2i>()->value = bounds.min;
			material.getUniform(maxUniformLocation).as<UniformVector2i>()->value = bounds.max;
		});
	}

	Sprite::~Sprite()
	{
		getScene()->destroyModel(model);
	}

	float Sprite::getDepth() const
	{
		return model->getDepth();
	}

	void Sprite::setDepth(float depth)
	{
		model->setDepth(depth);
	}

	Recti Sprite::getBounds() const
	{
		return bounds;
	}

	void Sprite::setBounds(Recti bounds_)
	{
		bounds = bounds_;
	}

	void Sprite::setImage(std::string const & name)
	{
		auto & texture = model->getMaterial()->getUniform("tex").as<UniformTexture2d>()->texture;
		texture = store.textures.get(name);
		if (!texture)
		{
			auto image = store.images.get(name);
			if (!image)
			{
				throw std::runtime_error("Image name '" + name + "' not found. ");
			}
			texture = store.textures.create(name, image);
		}	 
	}

	void Sprite::update(float dt)
	{
	}
}