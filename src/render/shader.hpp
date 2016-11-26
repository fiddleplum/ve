#pragma once

#include "util/matrix.hpp"
#include "util/config.hpp"
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

		// Constructs a shader from a config.
		Shader(Config const & config);

		// Constructs a shader from the name.
		Shader(std::string const & filename);

		// Destructor.
		~Shader();

		// Returns information on all of the uniforms.
		std::map<std::string, UniformInfo> const & getUniformInfos() const;

		// Returns the information on a uniform given the name.
		UniformInfo getUniformInfo(std::string const & name) const;

		// Sets a uniform value of the currently active shader. Value types for T are below this class definition.
		template <typename T> static void setUniformValue(int location, T const & value);

		// Returns the location of an attribute given the name.
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

	template <> void Shader::setUniformValue(int location, int const & value);
	template <> void Shader::setUniformValue(int location, float const & value);
	template <> void Shader::setUniformValue(int location, Vector2i const & value);
	template <> void Shader::setUniformValue(int location, Vector3i const & value);
	template <> void Shader::setUniformValue(int location, Vector4i const & value);
	template <> void Shader::setUniformValue(int location, Vector2f const & value);
	template <> void Shader::setUniformValue(int location, Vector3f const & value);
	template <> void Shader::setUniformValue(int location, Vector4f const & value);
	template <> void Shader::setUniformValue(int location, Matrix22f const & value);
	template <> void Shader::setUniformValue(int location, Matrix33f const & value);
	template <> void Shader::setUniformValue(int location, Matrix44f const & value);
}