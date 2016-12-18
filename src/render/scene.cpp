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

	void Scene::destroyModel(Ptr<Model> model)
	{
		auto it = std::find(models.begin(), models.end(), model);
		if (it == models.end())
		{
			assert(false);
			throw std::runtime_error("WHAT");
		}
		else
		{
			models.erase(it);
		}
	}

	void Scene::addUniformsFunction(int id, std::function<void(Ptr<Shader> const &)> uniformsFunction)
	{
		uniformsFunctions[id] = uniformsFunction;
	}

	void Scene::removeUniformsFunction(int id)
	{
		uniformsFunctions.erase(id);
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
			model->render(uniformsFunctions);
		}
	}
}