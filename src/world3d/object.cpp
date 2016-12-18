#include "object.hpp"

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
		}

		Object::~Object()
		{
			scene->destroyModel(model);
		}
	}
}