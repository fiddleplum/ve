#include "shader.h"
#include "mesh.h"
#include "open_gl.h"
#include "util.h"

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
	unsigned int currentProgram = 0; // maintains current open gl state

	Shader::Shader(std::string const & shaderCodeNames)
	{
		std::vector<std::string> shaderObjectCode(ShaderCode::NumCodeTypes);
		std::vector<std::string> shaderCodeNameList;
		split(shaderCodeNames, ',', shaderCodeNameList);
		for (auto shaderCodeName : shaderCodeNameList)
		{
			UsePtr<ShaderCode> shaderCode = ShaderCode::cache.get(shaderCodeName);
			if (!shaderCode.isValid())
			{
				shaderCode = ShaderCode::cache.create(shaderCodeName, shaderCodeName);
			}
			shaderObjectCode[shaderCode->getType()] += shaderCode->getCode();
		}
		std::vector<unsigned int> shaderObjects;
		try
		{
			for (unsigned int type = 0; type < NumCodeTypes; type++)
			{
				if (!shaderObjectCode[type].empty())
				{
					shaderObjects.push_back(compileShaderObject((CodeType)type, shaderObjectCode[type]));
				}
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
		populateVariableLocations();
	}

	Shader::~Shader()
	{
		glDeleteProgram(program);
	}

	int Shader::getUniformLocation(std::string const & name) const
	{
		auto it = uniformLocations.find(name);
		if (it == uniformLocations.end())
		{
			return -1;
		}
		return it->second;
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

	void Shader::activate()
	{
		if (currentProgram != program)
		{
			currentProgram = program;
			glUseProgram(program);
		}
	}

	void Shader::deactivate()
	{
		currentProgram = 0;
		glUseProgram(0);
	}

	void Shader::setUniform(int location, int value)
	{
		glUniform1i(location, value);
	}

	void Shader::setUniform(int location, float value)
	{
		glUniform1f(location, value);
	}

	void Shader::setUniform(int location, Vector2i value)
	{
		glUniform2iv(location, 1, value.ptr());
	}

	void Shader::setUniform(int location, Vector2f value)
	{
		glUniform2fv(location, 1, value.ptr());
	}

	void Shader::setUniform(int location, Vector3i value)
	{
		glUniform3iv(location, 1, value.ptr());
	}

	void Shader::setUniform(int location, Vector3f value)
	{
		glUniform3fv(location, 1, value.ptr());
	}

	void Shader::setUniform(int location, Vector4i value)
	{
		glUniform4iv(location, 1, value.ptr());
	}

	void Shader::setUniform(int location, Vector4f value)
	{
		glUniform4fv(location, 1, value.ptr());
	}

	void Shader::setUniform(int location, Matrix33f const & value)
	{
		glUniformMatrix3fv(location, 1, false, value.ptr());
	}

	void Shader::setUniform(int location, Matrix44f const & value)
	{
		glUniformMatrix4fv(location, 1, false, value.ptr());
	}

	void Shader::setUniform(int location, std::vector<Vector2f> const & value)
	{
		glUniform2fv(location, (GLsizei)value.size(), (GLfloat const *)&value[0]);
	}

	void Shader::setUniform(int location, std::vector<Vector3f> const & value)
	{
		glUniform3fv(location, (GLsizei)value.size(), (GLfloat const *)&value[0]);
	}

	unsigned int Shader::compileShaderObject(CodeType type, std::string const & code)
	{
		unsigned int glType = 0;
		if (type == Vertex)
		{
			glType = GL_VERTEX_SHADER;
		}
		else if (type == Fragment)
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

	void Shader::populateVariableLocations()
	{
		GLint numVariables;
		GLint maxNameSize;
		std::string name;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numVariables);
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
		for (int i = 0; i < numVariables; i++)
		{
			GLsizei nameSize;
			GLint size;
			GLenum type;
			name.resize(maxNameSize);
			glGetActiveUniform(program, i, maxNameSize, &nameSize, &size, &type, &name[0]);
			name.resize(nameSize);
			GLint location = glGetUniformLocation(program, name.c_str());
			if (location != -1)
			{
				uniformLocations[name] = location;
			}
		}
	}
}

