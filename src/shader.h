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
		enum CodeType { Vertex, Fragment, NumCodeTypes };

		// Constructs a shader from comma separated shader code file names.
		Shader(std::string const & shaderCodeNames);

		Shader(std::vector<ShaderCode> const & shaderCodes);

		~Shader();

		int getUniformLocation(std::string const & name) const;

		//int getUniformType(std::string const & name) const;

		int getAttributeLocation(std::string const & name) const;

		void activate();

		static void deactivate();

		static void setUniform(int location, int value);

		static void setUniform(int location, float value);

		static void setUniform(int location, Vector2i value);

		static void setUniform(int location, Vector2f value);

		static void setUniform(int location, Vector3i value);

		static void setUniform(int location, Vector3f value);

		static void setUniform(int location, Vector4i value);

		static void setUniform(int location, Vector4f value);

		static void setUniform(int location, Matrix33f const & value);

		static void setUniform(int location, Matrix44f const & value);

		static void setUniform(int location, std::vector<Vector2f> const & value);

		static void setUniform(int location, std::vector<Vector3f> const & value);

		static Cache<Shader> cache;

	private:
		static unsigned int compileShaderObject(CodeType type, std::string const & code);

		static unsigned int linkShaderProgram(std::vector<unsigned int> const & shaderObjects);

		void bindAttributeLocations();

		void populateVariableLocations();

		static std::map<std::string, int> attributeLocations;

		unsigned int program;
		std::map<std::string, int> uniformLocations;
	};
}

