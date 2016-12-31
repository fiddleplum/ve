#pragma once

#include "render/model.hpp"
#include "render/texture.hpp"
#include "render/vertex_buffer_object.hpp"
#include "render/shader.hpp"
#include "render/font.hpp"
#include "util/cache.hpp"
#include <unordered_set>
#include <unordered_map>
#include <set>

namespace ve
{
	class Stage;

	class Scene
	{
	public:
		//! Adds a stage that this scene depends on.
		void addDependentStage(Ptr<Stage> stage);

		//! Removes a stage that this scene no longer depends on.
		void removeDependentStage(Ptr<Stage> stage);

		//! Returns the set of stages that this scene depends on.
		std::set<Ptr<Stage>> const & getDependentStages() const;

		//! Creates a new model and returns it.
		Ptr<Model> createModel();

		//! Removes a model.
		void destroyModel(Ptr<Model> model);

		//! Sets the function to be called that sets any model-specific uniforms. Called every time the shader is changed.
		void setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction);

		//! Renders the scene.
		void render(std::function<void(Ptr<Shader> const &)> const & stageUniformsFunction);

	private:
		std::set<Ptr<Stage>> dependentStages;
		std::function<void(Ptr<Shader> const &)> uniformsFunction;
		std::unordered_set<OwnPtr<Model>> models;
	};
}