#pragma once

#include "render/model.hpp"
#include "render/texture.hpp"
#include "render/vertex_buffer_object.hpp"
#include "render/shader.hpp"
#include "render/material.hpp"
#include "render/font.hpp"
#include "util/cache.hpp"
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
		void setUniformsFunction(std::function<void(Ptr<Shader> const &)> uniformsFunction);

		// Renders the scene.
		void render();

	private:
		std::function<void(Ptr<Shader> const &)> uniformsFunction;
		std::unordered_set<OwnPtr<Model>> models;

	};
}