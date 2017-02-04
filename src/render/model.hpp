#pragma once

#include "render/mesh.hpp"
#include "render/image.hpp"
#include "render/shader.hpp"
#include "util/ptr.hpp"
#include <functional>
#include <unordered_map>

namespace ve
{
	namespace render
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
			Ptr<Mesh> getMesh() const;

			// Sets the used vertex buffer object.
			void setMesh(Ptr<Mesh> const & mesh);

			// Gets the shader.
			Ptr<Shader> getShader() const;

			// Sets the shader.
			void setShader(Ptr<Shader> const & shader);

			// Returns the image used at the given slot
			Ptr<Image> getImageAtSlot(unsigned int slot) const;

			// Sets the image used at the given slot.
			void setImageAtSlot(Ptr<Image> const & image, unsigned int slot);

			// Sets the function to be called that sets any model-specific uniforms.
			void setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction);

			// Renders the model.
			void render(std::function<void(Ptr<Shader> const &)> const & stageUniformsFunction, std::function<void(Ptr<Shader> const &)> const & sceneUniformsFunction, bool flipY) const;

			// Returns true if this model sorts less than the other model.
			bool operator < (Model const & model) const;

		private:
			float depth;
			std::vector<Ptr<Image>> images;
			Ptr<Shader> shader;
			Ptr<Mesh> mesh;
			std::function<void(Ptr<Shader> const &)> uniformsFunction;
		};

		bool operator < (Ptr<Model> const & lhs, Ptr<Model> const & rhs);
	}
}