#pragma once

#include "util/matrix.hpp"
#include "util/config.hpp"
#include <string>
#include <map>
#include <vector>

namespace ve
{
	namespace render
	{
		class Shader
		{
		public:
			// Blending options. This will get more complicated as time goes on.
			enum Blending { NONE, ADDITIVE, ALPHA };

			enum DepthTest { NEVER, ALWAYS, LESS, GREATER, EQUAL, NOT_EQUAL, LESS_OR_EQUAL, GREATER_OR_EQUAL };

			enum UniformType { INT, COORD_2I, COORD_3I, COORD_4I, FLOAT, COORD_2F, COORD_3F, COORD_4F, MATRIX_22F, MATRIX_33F, MATRIX_44F, TEXTURE_2D, TEMPLATE };

			// Uniform information for use by Materials.
			struct UniformInfo
			{
				int location;
				UniformType type;
			};

			struct Config
			{
				std::string vertexCode;
				std::string fragmentCode;
				Blending blending = Blending::NONE;
				bool depthWrite = false;
				DepthTest depthTest = DepthTest::LESS_OR_EQUAL;
			};

			//! Constructs a shader from a shader config.
			Shader(Config const & shaderConfig);

			// Constructs a shader from a config.
			Shader(ve::Config const & config);

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

			// Sets a uniform value of the currently active shader. Value types for T are below this class definition. Slower than using the location because of the string lookup.
			template <typename T> void setUniformValue(char const * name, T const & value);

			// Returns the location of an attribute given the name.
			int getAttributeLocation(std::string const & name) const;

			// Returns the blending state.
			Blending getBlending() const;

			// Sets the blending state.
			void setBlending(Blending blending);

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

			Blending blending;
			bool depthWrite;
			DepthTest depthTest;
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

		template <> void Shader::setUniformValue(char const * location, int const & value);
		template <> void Shader::setUniformValue(char const * location, float const & value);
		template <> void Shader::setUniformValue(char const * location, Vector2i const & value);
		template <> void Shader::setUniformValue(char const * location, Vector3i const & value);
		template <> void Shader::setUniformValue(char const * location, Vector4i const & value);
		template <> void Shader::setUniformValue(char const * location, Vector2f const & value);
		template <> void Shader::setUniformValue(char const * location, Vector3f const & value);
		template <> void Shader::setUniformValue(char const * location, Vector4f const & value);
		template <> void Shader::setUniformValue(char const * location, Matrix22f const & value);
		template <> void Shader::setUniformValue(char const * location, Matrix33f const & value);
		template <> void Shader::setUniformValue(char const * location, Matrix44f const & value);
	}
}