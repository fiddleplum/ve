#pragma once

#include "material.hpp"
#include "vertex_buffer_object.hpp"
#include <functional>

namespace ve
{
	// A model class contains all of the information needed to draw a single 'object' on the screen.
	class Model
	{
	public:
		// Returns the depth.
		float getDepth() const;

		// Sets the depth. Used by the Stage for sorting models.
		void setDepth(float depth);

		// Gets the material used by the model.
		Ptr<Material> getMaterial() const;

		// Sets the material used by the model.
		void setMaterial(Ptr<Material> material);

		// Gets the vertex buffer object used by the model.
		Ptr<VertexBufferObject> getVertexBufferObject() const;

		// Sets the vertex buffer object used by the model.
		void setVertexBufferObject(Ptr<VertexBufferObject> vertexBufferObject);

		// Sets the function to be called that sets any model-specific uniforms.
		void setUniformsFunction(std::function<void(Material const &)> uniformsFunction);

		// Renders the model.
		void render(std::function<void(Material const &)> const & sceneUniformsFunction) const;

		// Returns true if this model sorts less than the other model.
		bool operator < (Model const & model) const;

	private:
		float depth;
		Ptr<Material> material;
		Ptr<VertexBufferObject> vertexBufferObject;
		std::function<void(Material const &)> uniformsFunction;
	};

	bool operator < (Ptr<Model> const & lhs, Ptr<Model> const & rhs);
}