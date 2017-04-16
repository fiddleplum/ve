#include "world/object.hpp"
#include <fstream>

namespace ve
{
	namespace world
	{
		Object::Object(Ptr<render::Scene> const & scene_)
		{
			localToWorldTransformLocation = -1;
			scene = scene_;
			model = scene->createModel();
			model->setUniformsFunction([this](Ptr<render::Shader> const & shader)
			{
				if (localToWorldTransformLocation == -1)
				{
					localToWorldTransformLocation = shader->getUniformInfo("localToWorldTransform").location;
				}
				shader->setUniformValue(localToWorldTransformLocation, getLocalToWorldTransform());
			});
			mesh.setNew();
			model->setMesh(mesh);
			updateShader();
		}

		Object::Object(Ptr<render::Scene> const & scene, std::string const & filename)
			: Object(scene)
		{
			std::ifstream ifs {filename, std::ios::binary};
			mesh.setNew(ifs);
			model->setMesh(mesh);
			updateShader();
		}

		Object::~Object()
		{
			scene->destroyModel(model);
		}

		Ptr<render::Model> Object::getModel() const
		{
			return model;
		}

		void Object::updateShader()
		{
			//Ptr<render::Shader> shader = getStore()->shaders.get("object");
			//if (!shader)
			//{
			//	render::Shader::Config shaderConfig;
			//	shaderConfig.vertexCode =
			//		"#version 430\n"
			//		"attribute vec3 position3d;\n"
			//		"uniform mat4 localToWorldTransform;\n"
			//		"uniform mat4 worldToCameraTramsform;\n"
			//		"uniform mat4 cameraToNdcTransform;\n"
			//		"uniform float flipY;\n"
			//		"void main(void)\n"
			//		"{\n"
			//		"	gl_Position = cameraToNdcTransform * worldToCameraTramsform * localToWorldTransform * vec4(position3d, 1.0);\n"
			//		"	gl_Position.y *= flipY;\n"
			//		"}\n";
			//	shaderConfig.fragmentCode =
			//		"#version 430\n"
			//		"out vec4 color;\n"
			//		"void main(void)\n"
			//		"{\n"
			//		"	color = vec4(1.0, 1.0, 1.0, 1.0);\n"
			//		"}\n";
			//	shaderConfig.blending = render::Shader::Blending::NONE;
			//	shader = getStore()->shaders.create("object", shaderConfig);
			//}
			//auto shader = model->getShader();
			//if (shader.isValid())
			//{
			//	model->setShader(shader);
			//}
		}
	}
}