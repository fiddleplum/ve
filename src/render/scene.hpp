#pragma once

#include "render/model.hpp"
#include "render/texture.hpp"
#include "render/vertex_buffer_object.hpp"
#include "render/shader.hpp"
#include "render/material.hpp"
#include "render/font.hpp"
#include "util/cache.hpp"
#include <unordered_set>
#include <unordered_map>

namespace ve
{
	class Scene
	{
	public:
		// Creates a new model.
		Ptr<Model> createModel();

		// Removes a model.
		void destroyModel(Ptr<Model> model);

		// Adds a function to be called that sets any model-specific uniforms. Called every time the shader is changed. The id is used only for removal.
		void addUniformsFunction(int id, std::function<void(Ptr<Shader> const &)> uniformsFunction);

		// Removes the function above, using the id provided.
		void removeUniformsFunction(int id);

		// Renders the scene.
		void render();

	private:
		std::unordered_map<int, std::function<void(Ptr<Shader> const &)>> uniformsFunctions;
		std::unordered_set<OwnPtr<Model>> models;

	};
}