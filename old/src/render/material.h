#pragma once

#include "shader.h"
#include "uniform.h"

namespace ve
{
	namespace render
	{
		// The material class is used by the model to set the values of the uniforms in the shader.
		class Material
		{
		public:
			// Constructs the material.
			Material();

			// Destructs the material.
			~Material();

			// Gets the shader.
			UsePtr<Shader> getShader() const;

			// Sets the shader.
			void setShader(UsePtr<Shader> shader);

			// Gets a uniform of the given location. Faster than by name.
			UsePtr<Uniform> getUniform(int location);

			// Gets a uniform of the given name.
			UsePtr<Uniform> getUniform(std::string const & name);

			// Activates the material.
			void activate() const;

			// Returns true if this material sorts less than the other material.
			bool operator < (Material const & material) const;

		private:
			UsePtr<Shader> shader;
			std::map<int, OwnPtr<Uniform>> uniforms;
			std::vector<UsePtr<UniformTexture2d>> texture2dUniforms;
			int numTextures = 0;
		};

		// Compare one material to another.
		bool operator < (UsePtr<Material> const & lhs, UsePtr<Material> const & rhs);
	}
}

