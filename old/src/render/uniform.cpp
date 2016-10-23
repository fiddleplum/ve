#include "uniform.h"
#include "open_gl.h"

namespace ve
{
	namespace render
	{
		int Uniform::getLocation() const
		{
			return location;
		}

		int Uniform::getType() const
		{
			return type;
		}

		Uniform::Uniform(int type, int location) : type(type), location(location)
		{
		}

		UniformInt::UniformInt(int location) : Uniform(INT, location)
		{
		}

		void UniformInt::sendToActiveShader() const
		{
			glUniform1i(getLocation(), value);
		}

		UniformVector2i::UniformVector2i(int location) : Uniform(VECTOR_2I, location)
		{
		}

		void UniformVector2i::sendToActiveShader() const
		{
			glUniform2iv(getLocation(), 1, value.ptr());
		}

		UniformVector3i::UniformVector3i(int location) : Uniform(VECTOR_3I, location)
		{
		}

		void UniformVector3i::sendToActiveShader() const
		{
			glUniform3iv(getLocation(), 1, value.ptr());
		}

		UniformVector4i::UniformVector4i(int location) : Uniform(VECTOR_4I, location)
		{
		}

		void UniformVector4i::sendToActiveShader() const
		{
			glUniform4iv(getLocation(), 1, value.ptr());
		}

		UniformFloat::UniformFloat(int location) : Uniform(FLOAT, location)
		{
		}

		void UniformFloat::sendToActiveShader() const
		{
			glUniform1f(getLocation(), value);
		}

		UniformVector2f::UniformVector2f(int location) : Uniform(VECTOR_2F, location)
		{
		}

		void UniformVector2f::sendToActiveShader() const
		{
			glUniform2fv(getLocation(), 1, value.ptr());
		}

		UniformVector3f::UniformVector3f(int location) : Uniform(VECTOR_3F, location)
		{
		}

		void UniformVector3f::sendToActiveShader() const
		{
			glUniform3fv(getLocation(), 1, value.ptr());
		}

		UniformVector4f::UniformVector4f(int location) : Uniform(VECTOR_4F, location)
		{
		}

		void UniformVector4f::sendToActiveShader() const
		{
			glUniform4fv(getLocation(), 1, value.ptr());
		}

		UniformMatrix22f::UniformMatrix22f(int location) : Uniform(MATRIX_22F, location)
		{
		}

		void UniformMatrix22f::sendToActiveShader() const
		{
			glUniformMatrix2fv(getLocation(), 1, false, value.ptr());
		}

		UniformMatrix33f::UniformMatrix33f(int location) : Uniform(MATRIX_33F, location)
		{
		}

		void UniformMatrix33f::sendToActiveShader() const
		{
			glUniformMatrix3fv(getLocation(), 1, false, value.ptr());
		}

		UniformMatrix44f::UniformMatrix44f(int location) : Uniform(MATRIX_44F, location)
		{
		}

		void UniformMatrix44f::sendToActiveShader() const
		{
			glUniformMatrix4fv(getLocation(), 1, false, value.ptr());
		}

		UniformTexture2d::UniformTexture2d(int location, int textureSlot_) : Uniform(TEXTURE_2D, location), textureSlot(textureSlot_)
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
}

