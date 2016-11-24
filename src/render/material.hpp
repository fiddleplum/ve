#pragma once

#include "shader.hpp"
#include "uniform.hpp"

namespace ve
{
	// The material class is used by the model to set the values of the uniforms in the shader.
	class Material
	{
	public:
		// Constructs the material.
		Material();

		// Constructs the material from a file.
		Material(std::string const & filename);

		// Destructs the material.
		~Material();

		// Gets the shader.
		Ptr<Shader> getShader() const;

		// Sets the shader.
		void setShader(Ptr<Shader> shader);

		// Gets a uniform of the given location. Faster than by name.
		Ptr<Uniform> getUniform(int location) const;

		// Gets a uniform of the given name.
		Ptr<Uniform> getUniform(std::string const & name) const;

		// Activates the material. Applies scene uniform values if the shader has been activated.
		void activate(std::function<void(Material const &)> const & sceneUniformsFunction, std::function<void(Material const &)> const & modelUniformsFunction) const;

		// Returns true if this material sorts less than the other material.
		bool operator < (Material const & material) const;

	private:
		Ptr<Shader> shader;
		std::map<int, OwnPtr<Uniform>> uniforms;
		std::vector<Ptr<UniformTexture2d>> texture2dUniforms;
		int numTextures = 0;
	};

	// Compare one material to another.
	bool operator < (Ptr<Material> const & lhs, Ptr<Material> const & rhs);
}