#pragma once

#include "render/model.h"
#include "util/ptr.h"
#include <unordered_set>

namespace ve
{
	class Scene
	{
	public:
		// Creates a new model.
		Ptr<Model> createModel();

		// Removes a model.
		void destroyModel(Ptr<Model> & model);

		// Renders the scene.
		void render();

	private:
		std::unordered_set<OwnPtr<Model>> models;
	};
}