#pragma once

#include "material.hpp"
#include "vertex_buffer_object.hpp"
#include <functional>
#include <unordered_map>

namespace ve
{
	// A model class contains all of the information needed to draw a single 'object' on the screen.
	class Model
	{
	public:
		// Default constructor.
		Model();

		// Returns the depth.
		float getDepth() const;

		// Sets the depth. Used by the Stage for sorting models.
		void setDepth(float depth);

		// Returns the used vertex buffer object.
		Ptr<VertexBufferObject> getVertexBufferObject() const;

		// Sets the used vertex buffer object.
		void setVertexBufferObject(Ptr<VertexBufferObject> const & vertexBufferObject);

		// Gets the shader.
		Ptr<Shader> getShader() const;

		// Sets the shader.
		void setShader(Ptr<Shader> const & shader);

		// Returns the texture used at the given slot
		Ptr<Texture> getTextureAtSlot(unsigned int slot) const;

		// Sets the texture used at the given slot.
		void setTextureAtSlot(Ptr<Texture> const & texture, unsigned int slot);

		// Sets the function to be called that sets any model-specific uniforms.
		void setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction);

		// Renders the model.
		void render(std::function<void(Ptr<Shader> const &)> const & stageUniformsFunction, std::function<void(Ptr<Shader> const &)> const & sceneUniformsFunction) const;

		// Returns true if this model sorts less than the other model.
		bool operator < (Model const & model) const;

	private:
		float depth;
		std::vector<Ptr<Texture>> textures;
		Ptr<Shader> shader;
		Ptr<VertexBufferObject> vertexBufferObject;
		std::function<void(Ptr<Shader> const &)> uniformsFunction;
	};

	bool operator < (Ptr<Model> const & lhs, Ptr<Model> const & rhs);
}