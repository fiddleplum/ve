#include "vector.h"
#include "matrix.h"
#include <vector>

namespace ve
{
	// An OpenGL uniform used by Shader and Material.
	class Uniform
	{
	public:
		// Constructor.
		Uniform(int type);

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader() = 0;

		// The value to be sent to the Shader.
		int location;

		// The type of the variable.
		int const type;

		// The available types.
		enum Type { INT, FLOAT, VECTOR_2I, VECTOR_2F, VECTOR_3I, VECTOR_3F, VECTOR_4I, VECTOR_4F, MATRIX_33F, MATRIX_44F };
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformInt : public Uniform
	{
	public:
		// Constructor.
		UniformInt();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		int value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformFloat : public Uniform
	{
	public:
		// Constructor.
		UniformFloat();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		float value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector2i : public Uniform
	{
	public:
		// Constructor.
		UniformVector2i();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Vector2i value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector2f : public Uniform
	{
	public:
		// Constructor.
		UniformVector2f();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Vector2f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector3i : public Uniform
	{
	public:
		// Constructor.
		UniformVector3i();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Vector3i value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector3f : public Uniform
	{
	public:
		// Constructor.
		UniformVector3f();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Vector3f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector4i : public Uniform
	{
	public:
		// Constructor.
		UniformVector4i();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Vector4i value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector4f : public Uniform
	{
	public:
		// Constructor.
		UniformVector4f();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Vector4f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformMatrix33f : public Uniform
	{
	public:
		// Constructor.
		UniformMatrix33f();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Matrix33f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformMatrix44f : public Uniform
	{
	public:
		// Constructor.
		UniformMatrix44f();

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader();

		// The value to be sent to the Shader.
		Matrix44f value;
	};
}

