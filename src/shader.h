#pragma once

#include "vector.h"
#include "matrix.h"
#include "cache.h"
#include "shader_code.h"
#include <string>
#include <map>
#include <vector>

namespace ve
{
	class Shader
	{
	public:
		// Uniform information for use by Materials.
		struct UniformInfo
		{
			int location;
			int type;
		};

		// Constructs a shader from comma separated shader code file names.
		Shader(std::string const & shaderCodeNames);

		// Destructor.
		~Shader();

		// Returns information on all of the uniforms.
		std::map<std::string, UniformInfo> const & getUniformInfos() const;

		UniformInfo getUniformInfo(std::string const & name) const;

		int getAttributeLocation(std::string const & name) const;

		void activate();

		static void deactivate();

		static Cache<Shader> cache;

	private:
		static unsigned int compileShaderObject(int type, std::string const & code);

		static unsigned int linkShaderProgram(std::vector<unsigned int> const & shaderObjects);

		void bindAttributeLocations();

		void populateUniformInfos();

		std::map<std::string, UniformInfo> uniformInfos;

		unsigned int program;
	};
}

