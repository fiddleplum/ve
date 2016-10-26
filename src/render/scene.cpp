#include "scene.h"
#include <algorithm>

namespace ve
{
	UsePtr<Model> Scene::createModel()
	{
		auto model = OwnPtr<Model>::createNew();
		models.insert(model);
		return model;
	}

	void Scene::destroyModel(UsePtr<Model> & model)
	{
		auto it = std::lower_bound(models.begin(), models.end(), model);
		if (*it == model)
		{
			models.erase(it);
		}
	}
}