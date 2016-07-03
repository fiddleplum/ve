#include "shader.h"
#include "uniform.h"
#include "ptr.h"

// NOTES:
/*
	If this class is just an abstract class, and it doesn't do very much and every model class will just have a material class,
	I think I shouldn't even have this class at all. This logic should be in a specific model class (2D or 3D).

	Should the model class be abstract and inherited? I feel no. 
*/

namespace ve
{
	// The abstract material class is used by the model to set the values of the uniforms in the shader.
	class Material
	{
	public:
		// Constructs the material.
		Material();

		// Destructs the material.
		virtual ~Material();

		// Gets the shader.
		UsePtr<Shader> getShader() const;

		// Sets the shader.
		void setShader(UsePtr<Shader> shader);

		// Activates the material.
		void activate() const;

	private:
		UsePtr<Shader> shader;

		std::map<int, OwnPtr<Uniform>> uniforms;
	};
}

