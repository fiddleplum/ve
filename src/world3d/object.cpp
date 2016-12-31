#include "object.hpp"
#include "../store.hpp"
#include <fstream>

namespace ve
{
	namespace world3d
	{
		Object::Object(Ptr<render::Scene> const & scene_)
		{
			scene = scene_;
			model = scene->createModel();
			model->setUniformsFunction([this](Ptr<render::Shader> const & shader)
			{
				shader->setUniformValue(localToWorldTransformLocation, getLocalToWorldTransform());
			});
			//updateShader();
		}

		Object::Object(Ptr<render::Scene> const & scene, std::string const & filename)
			: Object(scene)
		{
			std::ifstream ifs {filename, std::ios::binary};
			Mesh mesh {ifs};
			vbo.setNew(mesh);
			model->setVertexBufferObject(vbo);
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
			Ptr<render::Shader> shader = store.shaders.get("object");
			if (!shader)
			{
				render::Shader::Config shaderConfig;
				shaderConfig.vertexCode =
					"attribute vec3 position3d;\n"
					"uniform mat4 localToWorldTransform;\n"
					"uniform mat4 worldToCameraTramsform;\n"
					"uniform mat4 cameraToNdcTransform;\n"
					"void main(void)\n"
					"{\n"
					"	gl_Position = cameraToNdcTransform * worldToCameraTramsform * localToWorldTransform * vec4(position3d, 1.0);\n"
					"}\n";
				shaderConfig.fragmentCode =
					"void main(void)\n"
					"{\n"
					"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
					"}\n";
				shaderConfig.blending = render::Shader::Blending::NONE;
				shader = store.shaders.create("object", shaderConfig);
			}
			localToWorldTransformLocation = shader->getUniformInfo("localToWorldTransform").location;
			model->setShader(shader);
		}
	}
}