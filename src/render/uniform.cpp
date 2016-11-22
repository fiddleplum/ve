#include "uniform.h"
#include "open_gl.h"

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
		glUniform1i(getLocation(), value);
	}

	UniformVector2i::UniformVector2i(std::string const & name, int location) : Uniform(name, COORD_2I, location)
	{
	}

	void UniformVector2i::sendToActiveShader() const
	{
		glUniform2iv(getLocation(), 1, value.ptr());
	}

	UniformVector3i::UniformVector3i(std::string const & name, int location) : Uniform(name, COORD_3I, location)
	{
	}

	void UniformVector3i::sendToActiveShader() const
	{
		glUniform3iv(getLocation(), 1, value.ptr());
	}

	UniformVector4i::UniformVector4i(std::string const & name, int location) : Uniform(name, COORD_4I, location)
	{
	}

	void UniformVector4i::sendToActiveShader() const
	{
		glUniform4iv(getLocation(), 1, value.ptr());
	}

	UniformFloat::UniformFloat(std::string const & name, int location) : Uniform(name, FLOAT, location)
	{
	}

	void UniformFloat::sendToActiveShader() const
	{
		glUniform1f(getLocation(), value);
	}

	UniformVector2f::UniformVector2f(std::string const & name, int location) : Uniform(name, COORD_2F, location)
	{
	}

	void UniformVector2f::sendToActiveShader() const
	{
		glUniform2fv(getLocation(), 1, value.ptr());
	}

	UniformVector3f::UniformVector3f(std::string const & name, int location) : Uniform(name, COORD_3F, location)
	{
	}

	void UniformVector3f::sendToActiveShader() const
	{
		glUniform3fv(getLocation(), 1, value.ptr());
	}

	UniformVector4f::UniformVector4f(std::string const & name, int location) : Uniform(name, COORD_4F, location)
	{
	}

	void UniformVector4f::sendToActiveShader() const
	{
		glUniform4fv(getLocation(), 1, value.ptr());
	}

	UniformMatrix22f::UniformMatrix22f(std::string const & name, int location) : Uniform(name, MATRIX_22F, location)
	{
	}

	void UniformMatrix22f::sendToActiveShader() const
	{
		glUniformMatrix2fv(getLocation(), 1, false, value.ptr());
	}

	UniformMatrix33f::UniformMatrix33f(std::string const & name, int location) : Uniform(name, MATRIX_33F, location)
	{
	}

	void UniformMatrix33f::sendToActiveShader() const
	{
		glUniformMatrix3fv(getLocation(), 1, false, value.ptr());
	}

	UniformMatrix44f::UniformMatrix44f(std::string const & name, int location) : Uniform(name, MATRIX_44F, location)
	{
	}

	void UniformMatrix44f::sendToActiveShader() const
	{
		glUniformMatrix4fv(getLocation(), 1, false, value.ptr());
	}

	UniformTexture2d::UniformTexture2d(std::string const & name, int location, int textureSlot_) : Uniform(name, TEXTURE_2D, location), textureSlot(textureSlot_)
	{
	}

	void UniformTexture2d::sendToActiveShader() const
	{
		if (texture)
		{
			texture->activate(textureSlot);
			glUniform1i(getLocation(), textureSlot);
		}
	}
}