#include "scene.hpp"
#include <set>
#include <algorithm>

namespace ve
{
	Ptr<Model> Scene::createModel()
	{
		auto model = OwnPtr<Model>::returnNew();
		models.insert(model);
		return model;
	}

	void Scene::destroyModel(Ptr<Model> & model)
	{
		auto it = std::lower_bound(models.begin(), models.end(), model);
		if (*it == model)
		{
			model.setNull();
			models.erase(it);
		}
	}

	void Scene::setUniformsFunction(std::function<void(Ptr<Shader> const &)> uniformsFunction_)
	{
		uniformsFunction = uniformsFunction_;
	}

	void Scene::render()
	{
		std::multiset<Ptr<Model>> modelsSorted;
		for (auto model : models)
		{
			modelsSorted.insert(model);
		}
		for (auto model : modelsSorted)
		{
			model->render(uniformsFunction);
		}
	}
}