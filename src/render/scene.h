#pragma once

#include "model.h"
#include "ptr.h"
#include <unordered_set>

namespace ve
{
	class Scene
	{
	public:
		// Creates a new model.
		UsePtr<Model> createModel();

		// Removes a model.
		void destroyModel(UsePtr<Model> & model);

	private:
		std::unordered_set<OwnPtr<Model>> models;
	};
}