#include "uniform.h"
#include "open_gl.h"

namespace ve
{
	Uniform::Uniform(int type) : type(type)
	{
	}

	UniformInt::UniformInt() : Uniform(INT)
	{
	}

	void UniformInt::sendToActiveShader()
	{
		glUniform1i(location, value);
	}

	UniformFloat::UniformFloat() : Uniform(FLOAT)
	{
	}

	void UniformFloat::sendToActiveShader()
	{
		glUniform1f(location, value);
	}

	UniformVector2i::UniformVector2i() : Uniform(VECTOR_2I)
	{
	}

	void UniformVector2i::sendToActiveShader()
	{
		glUniform2iv(location, 1, value.ptr());
	}

	UniformVector2f::UniformVector2f() : Uniform(VECTOR_2F)
	{
	}

	void UniformVector2f::sendToActiveShader()
	{
		glUniform2fv(location, 1, value.ptr());
	}

	UniformVector3i::UniformVector3i() : Uniform(VECTOR_3I)
	{
	}

	void UniformVector3i::sendToActiveShader()
	{
		glUniform3iv(location, 1, value.ptr());
	}

	UniformVector3f::UniformVector3f() : Uniform(VECTOR_3F)
	{
	}

	void UniformVector3f::sendToActiveShader()
	{
		glUniform3fv(location, 1, value.ptr());
	}

	UniformVector4i::UniformVector4i() : Uniform(VECTOR_4I)
	{
	}

	void UniformVector4i::sendToActiveShader()
	{
		glUniform4iv(location, 1, value.ptr());
	}

	UniformVector4f::UniformVector4f() : Uniform(VECTOR_4F)
	{
	}

	void UniformVector4f::sendToActiveShader()
	{
		glUniform4fv(location, 1, value.ptr());
	}

	UniformMatrix33f::UniformMatrix33f() : Uniform(MATRIX_33F)
	{
	}

	void UniformMatrix33f::sendToActiveShader()
	{
		glUniformMatrix3fv(location, 1, false, value.ptr());
	}

	UniformMatrix44f::UniformMatrix44f() : Uniform(MATRIX_44F)
	{
	}

	void UniformMatrix44f::sendToActiveShader()
	{
		glUniformMatrix4fv(location, 1, false, value.ptr());
	}
}

