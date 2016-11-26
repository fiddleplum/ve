#pragma once

#include "render/texture.hpp"
#include "util/vector.hpp"
#include "util/matrix.hpp"
#include "util/ptr.hpp"

namespace ve
{
	// An OpenGL uniform used by Shader and Material.
	class Uniform
	{
	public:
		// Constructor.
		Uniform(std::string const & name, int type, int location);

		// Returns the location.
		int getLocation() const;

		// Returns the type.
		int getType() const;

		// Send the value to the location in the currently active Shader.
		virtual void sendToActiveShader() const = 0;

		// The available types.
		enum Type { INT, COORD_2I, COORD_3I, COORD_4I, FLOAT, COORD_2F, COORD_3F, COORD_4F, MATRIX_22F, MATRIX_33F, MATRIX_44F, TEXTURE_2D, TEMPLATE };

	private:
		// The name of the uniform in the shader.
		std::string name;

		// The GL location of the uniform within the shader.
		int const location;

		// The type of the variable.
		int const type;
	};

	template <typename T>
	class UniformT : public Uniform
	{
		// Constructor.
		UniformT(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		T value;
	};

	// Constructor.
	template <typename T> UniformT<T>::UniformT(std::string const & name, int location)
		: Uniform(name, TEMPLATE, location)
	{

	}

	// Send the value to the location in the currently active Shader.
	template <typename T> void UniformT<T>::sendToActiveShader() const
	{
		Shader::setUniformValue(getLocation(), value);
	}

	// An OpenGL uniform used by Shader and Material.
	class UniformInt : public Uniform
	{
	public:
		// Constructor.
		UniformInt(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		int value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector2i : public Uniform
	{
	public:
		// Constructor.
		UniformVector2i(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Vector2i value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector3i : public Uniform
	{
	public:
		// Constructor.
		UniformVector3i(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Vector3i value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector4i : public Uniform
	{
	public:
		// Constructor.
		UniformVector4i(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Vector4i value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformFloat : public Uniform
	{
	public:
		// Constructor.
		UniformFloat(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		float value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector2f : public Uniform
	{
	public:
		// Constructor.
		UniformVector2f(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Vector2f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector3f : public Uniform
	{
	public:
		// Constructor.
		UniformVector3f(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Vector3f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformVector4f : public Uniform
	{
	public:
		// Constructor.
		UniformVector4f(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Vector4f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformMatrix22f : public Uniform
	{
	public:
		// Constructor.
		UniformMatrix22f(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Matrix22f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformMatrix33f : public Uniform
	{
	public:
		// Constructor.
		UniformMatrix33f(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Matrix33f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformMatrix44f : public Uniform
	{
	public:
		// Constructor.
		UniformMatrix44f(std::string const & name, int location);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The value to be sent to the Shader.
		Matrix44f value;
	};

	// An OpenGL uniform used by Shader and Material.
	class UniformTexture2d : public Uniform
	{
	public:
		// Constructor.
		UniformTexture2d(std::string const & name, int location, int textureSlot);

		// Send the value to the location in the currently active Shader.
		void sendToActiveShader() const override;

		// The texture slot that will be used.
		int const textureSlot;

		// The texture that will be sampled by the sampler.
		Ptr<Texture> texture;
	};
}