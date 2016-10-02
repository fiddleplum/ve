#include "shader.h"
#include "open_gl.h"
#include "uniform.h"
#include "../mesh.h"
#include "../stringutil.h"

// TODO: Make the shader use specific attribute locations chosen by an enum based on the attribute names.
// This allows multiple shaders to be used with a single vbo and all be compatible.

// A vbo is separate from the shaders, so that the same vbo can use multiple shaders and multiple vbos can use the same shader,
//   so the attributes need to be set by an enum that both vbos and shaders use
// A material, on the other hand, uses uniforms. uniforms can be any sort of name and type so that there can't really be a list of uniform ids
//   that both materials and shaders use. so a material must have a shader reference and whenever it is assigned a new shader, repopulate the
//   uniform ids. this makes sense, since it has to repopulate the uniform values anyway

// There SHOULD be a shared material, since many objects might want to use the same material, and it would be wasteful not to. then the objects
//   could be sorted by material (shader and then texture) when rendering. then a scene object would contain a mesh and a material, as shared resources.
//   the camera, when it needs to set the proj mat, would do so when the material is changed when rendering
//   the material would have some basic preset uniforms (proj, worldview, lights, etc) and their locations, and then the rest would be material properties

// I should put vbo inside of mesh, and have logic to keep the vertex/index data in memory or to get rid of it.

namespace ve
{
	namespace render
	{
		unsigned int currentProgram = 0; // maintains current open gl state
		std::map<std::string, int> attributeLocations = { // mapping from attribute names as strings to their corresponding bound locations
			{"position3d", Mesh::POSITION_3D},
			{"position2d", Mesh::POSITION_2D},
			{"normal", Mesh::NORMAL},
			{"tangent", Mesh::TANGENT},
			{"color0rgb", Mesh::COLOR0_RGB},
			{"color0rgba", Mesh::COLOR0_RGBA},
			{"color1rgb", Mesh::COLOR1_RGB},
			{"color1rgba", Mesh::COLOR1_RGBA},
			{"uv0", Mesh::UV0},
			{"uv0", Mesh::UV1},
			{"uv0", Mesh::UV2},
			{"uv0", Mesh::UV3}
		};

		Shader::Shader(Config const & config)
		{
			std::vector<unsigned int> shaderObjects;

			try
			{
				auto vertexValueOpt = config["vertex"];
				if (vertexValueOpt && vertexValueOpt->type == Config::List)
				{
					auto vertexValue = vertexValueOpt.value();
					for (auto pair : vertexValue.children)
					{
						auto filename = pair.second.text;
						auto code = readFile(filename);
						shaderObjects.push_back(compileShaderObject(Vertex, code));
					}
				}
				else
				{
					throw std::runtime_error("Shader definition does not have a vertex section. ");
				}

				auto fragmentValueOpt = config["fragment"];
				if (fragmentValueOpt && fragmentValueOpt->type == Config::List)
				{
					auto fragmentValue = fragmentValueOpt.value();
					for (auto pair : fragmentValue.children)
					{
						auto filename = pair.second.text;
						auto code = readFile(filename);
						shaderObjects.push_back(compileShaderObject(Fragment, code));
					}
				}
				else
				{
					throw std::runtime_error("Shader definition does not have a fragment section. ");
				}
			}
			catch (...)
			{
				for (unsigned int shaderObject : shaderObjects)
				{
					glDeleteShader(shaderObject);
				}
				throw;
			}
			program = linkShaderProgram(shaderObjects); // delete shader objects as well
			populateUniformInfos();
		}

		Shader::~Shader()
		{
			glDeleteProgram(program);
		}

		std::map<std::string, Shader::UniformInfo> const & Shader::getUniformInfos() const
		{
			return uniformInfos;
		}

		Shader::UniformInfo Shader::getUniformInfo(std::string const & name) const
		{
			auto iter = uniformInfos.find(name);
			if (iter != uniformInfos.end())
			{
				return iter->second;
			}
			else
			{
				throw std::runtime_error("The uniform '" + name + "' not found in the shader. ");
			}
		}

		int Shader::getAttributeLocation(std::string const & name) const
		{
			auto it = attributeLocations.find(name);
			if (it == attributeLocations.end())
			{
				return -1;
			}
			return it->second;
		}

		bool Shader::activate()
		{
			if (currentProgram != program)
			{
				currentProgram = program;
				glUseProgram(program);
				return true;
			}
			return false;
		}

		void Shader::deactivate()
		{
			currentProgram = 0;
			glUseProgram(0);
		}

		unsigned int Shader::compileShaderObject(Type type, std::string const & code)
		{
			unsigned int glType = 0;
			if (type == Shader::Vertex)
			{
				glType = GL_VERTEX_SHADER;
			}
			else if (type == Shader::Fragment)
			{
				glType = GL_FRAGMENT_SHADER;
			}
			else
			{
				throw std::runtime_error("Unknown shader object type '" + std::to_string(type) + "', with code:\n" + code + "\n");
			}
			unsigned int handle;
			handle = glCreateShader(glType);
			char const * shaderCode = code.c_str();
			GLint shaderCodeSize = (GLint)code.size();
			glShaderSource(handle, 1, &shaderCode, &shaderCodeSize);
			glCompileShader(handle);
			GLint good;
			glGetShaderiv(handle, GL_COMPILE_STATUS, &good);
			if (good == GL_FALSE)
			{
				GLint logLength;
				std::string log;
				glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
				log.resize(logLength);
				glGetShaderInfoLog(handle, logLength, 0, &log[0]);
				log.pop_back(); // get rid of \0
				std::string typeString;
				glDeleteShader(handle);
				throw std::runtime_error("Error compiling shader: " + log + "Code:\n" + code + "\n");
			}
			return handle;
		}

		unsigned int Shader::linkShaderProgram(std::vector<unsigned int> const & shaderObjects)
		{
			unsigned int program = glCreateProgram();
			for (unsigned int shaderObject : shaderObjects)
			{
				glAttachShader(program, shaderObject);
			}
			glLinkProgram(program);
			for (unsigned int shaderObject : shaderObjects)
			{
				glDetachShader(program, shaderObject);
				glDeleteShader(shaderObject);
			}
			GLint good;
			glGetProgramiv(program, GL_LINK_STATUS, &good);
			if (good == GL_FALSE)
			{
				GLint logLength;
				std::string log;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
				log.resize(logLength);
				glGetProgramInfoLog(program, logLength, 0, &log[0]);
				glDeleteProgram(program);
				throw std::runtime_error("Error linking shader: " + log);
			}
			return program;
		}

		void Shader::bindAttributeLocations()
		{
			GLint numVariables;
			GLint maxNameSize;
			std::string name;
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numVariables);
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameSize);
			for (int i = 0; i < numVariables; i++)
			{
				GLsizei nameSize;
				GLint size;
				GLenum type;
				name.resize(maxNameSize);
				glGetActiveAttrib(program, i, maxNameSize, &nameSize, &size, &type, &name[0]);
				name.resize(nameSize);
				if (attributeLocations.find(name) != attributeLocations.end())
				{
					GLint location = attributeLocations[name];
					glBindAttribLocation(program, location, name.c_str());
				}
				else
				{
					throw std::runtime_error("Invalid attribute name in shader. The name may only be one of the predefined attributes in Shader::Attribute. ");
				}
			}
			glLinkProgram(program); // need to relink after attributes are bound. No checking of errors here, because it was already linked successfully once.
		}

		void Shader::populateUniformInfos()
		{
			GLint numVariables;
			GLint maxNameSize;
			std::string name;
			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numVariables);
			glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
			for (int i = 0; i < numVariables; i++)
			{
				GLsizei nameSize;
				GLint glSize;
				GLenum glType;
				name.resize(maxNameSize);
				glGetActiveUniform(program, i, maxNameSize, &nameSize, &glSize, &glType, &name[0]);
				name.resize(nameSize);
				int location = glGetUniformLocation(program, name.c_str());
				int type = 0;
				switch (glType)
				{
					case GL_INT: type = Uniform::INT; break;
					case GL_FLOAT: type = Uniform::FLOAT; break;
					case GL_INT_VEC2: type = Uniform::VECTOR_2I; break;
					case GL_FLOAT_VEC2: type = Uniform::VECTOR_2F; break;
					case GL_INT_VEC3: type = Uniform::VECTOR_3I; break;
					case GL_FLOAT_VEC3: type = Uniform::VECTOR_3F; break;
					case GL_INT_VEC4: type = Uniform::VECTOR_4I; break;
					case GL_FLOAT_VEC4: type = Uniform::VECTOR_4F; break;
					case GL_FLOAT_MAT3: type = Uniform::MATRIX_33F; break;
					case GL_FLOAT_MAT4: type = Uniform::MATRIX_44F; break;
					case GL_SAMPLER_2D: type = Uniform::TEXTURE_2D; break;
					default: throw std::runtime_error("Invalid type of uniform for '" + name + "'. "); break;
				}
				uniformInfos[name] = UniformInfo {location, type};
			}
		}
	}
}

