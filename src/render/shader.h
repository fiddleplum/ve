#pragma once

#include "util/matrix.h"
#include "util/config.h"
#include <string>
#include <map>
#include <vector>

namespace ve
{
	class Shader
	{
	public:
		// Constructs a shader from a config.
		Shader(Config const & config);

		// Constructs a shader from the name.
		Shader(std::string const & filename);

		// Destructor.
		~Shader();

		// Uniform information for use by Materials.
		struct UniformInfo
		{
			int location;
			int type;
		};

		// Returns information on all of the uniforms.
		std::map<std::string, UniformInfo> const & getUniformInfos() const;

		// Returns the information on a uniform given the name.
		UniformInfo getUniformInfo(std::string const & name) const;

		int getAttributeLocation(std::string const & name) const;

		// Activates the current shader and returns true. If it was already activated, returns false.
		bool activate();

		// Deactivates the current shader.
		static void deactivate();

	private:
		enum Type { Vertex, Fragment, NumCodeTypes };

		static unsigned int compileShaderObject(Type type, std::string const & code);

		static unsigned int linkShaderProgram(std::vector<unsigned int> const & shaderObjects);

		void bindAttributeLocations();

		void populateUniformInfos();

		std::map<std::string, UniformInfo> uniformInfos;

		unsigned int program;
	};
}