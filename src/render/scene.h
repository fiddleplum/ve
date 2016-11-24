#pragma once

#include "render/model.h"
#include "render/texture.h"
#include "render/vertex_buffer_object.h"
#include "render/shader.h"
#include "render/material.h"
#include "render/font.h"
#include "util/cache.h"
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

		// Sets the function to be called that sets any model-specific uniforms. Called every time the shader is changed.
		void setUniformsFunction(std::function<void(Material const &)> uniformsFunction);

		// Renders the scene.
		void render();

	private:
		std::function<void(Material const &)> uniformsFunction;
		std::unordered_set<OwnPtr<Model>> models;

	};
}