#include "uniform.hpp"
#include "shader.hpp"
#include "open_gl.hpp"

namespace ve
{
	Uniform::Uniform(std::string const & name, int type, int location) : name(name), type(type), location(location)
	{
	}

	int Uniform::getLocation() const
	{
		return location;
	}

	int Uniform::getType() const
	{
		return type;
	}

	UniformInt::UniformInt(std::string const & name, int location) : Uniform(name, INT, location)
	{
	}

	void UniformInt::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformVector2i::UniformVector2i(std::string const & name, int location) : Uniform(name, COORD_2I, location)
	{
	}

	void UniformVector2i::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformVector3i::UniformVector3i(std::string const & name, int location) : Uniform(name, COORD_3I, location)
	{
	}

	void UniformVector3i::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformVector4i::UniformVector4i(std::string const & name, int location) : Uniform(name, COORD_4I, location)
	{
	}

	void UniformVector4i::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformFloat::UniformFloat(std::string const & name, int location) : Uniform(name, FLOAT, location)
	{
	}

	void UniformFloat::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformVector2f::UniformVector2f(std::string const & name, int location) : Uniform(name, COORD_2F, location)
	{
	}

	void UniformVector2f::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformVector3f::UniformVector3f(std::string const & name, int location) : Uniform(name, COORD_3F, location)
	{
	}

	void UniformVector3f::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformVector4f::UniformVector4f(std::string const & name, int location) : Uniform(name, COORD_4F, location)
	{
	}

	void UniformVector4f::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformMatrix22f::UniformMatrix22f(std::string const & name, int location) : Uniform(name, MATRIX_22F, location)
	{
	}

	void UniformMatrix22f::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformMatrix33f::UniformMatrix33f(std::string const & name, int location) : Uniform(name, MATRIX_33F, location)
	{
	}

	void UniformMatrix33f::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformMatrix44f::UniformMatrix44f(std::string const & name, int location) : Uniform(name, MATRIX_44F, location)
	{
	}

	void UniformMatrix44f::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	UniformTexture2d::UniformTexture2d(std::string const & name, int location, int textureSlot_) : Uniform(name, TEXTURE_2D, location), textureSlot(textureSlot_)
	{
	}

	void UniformTexture2d::sendToActiveShader() const
	{
		if (texture)
		{
			texture->activate(textureSlot);
			Shader::setUniformValue(getLocation(), textureSlot);
		}
	}
}