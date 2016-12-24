#include "object.hpp"
#include "../store.hpp"

namespace ve
{
	namespace world3d
	{
		Object::Object(Ptr<Scene> scene_)
		{
			scene = scene_;
			model = scene->createModel();
			model->setUniformsFunction([this](Ptr<Shader> const & shader)
			{
				shader->setUniformValue(localToWorldTransformLocation, getLocalToWorldTransform());
			});
			Ptr<Shader> shader = store.shaders.create("shader", "shaders/basic.shader");
			localToWorldTransformLocation = shader->getUniformInfo("uL2WMatrix").location;
			model->setShader(shader);
			Mesh mesh;
			mesh.formatTypes = {Mesh::POSITION_3D};
			mesh.vertices = {0, 0, 0, 1, 0, 0, 0, 1, 0};
			mesh.indices = {0, 1, 2};
			vbo.setNew(mesh);
			model->setVertexBufferObject(vbo);
		}

		Object::~Object()
		{
			scene->destroyModel(model);
		}
	}
}