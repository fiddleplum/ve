#pragma once

#include "texture.h"
#include "../vector.h"
#include "../matrix.h"
#include "../ptr.h"

namespace ve
{
	namespace render
	{
		// An OpenGL uniform used by Shader and Material.
		class Uniform
		{
		public:
			// Constructor.
			Uniform(int type, int location);

			// Returns the location.
			int getLocation() const;

			// Returns the type.
			int getType() const;

			// Send the value to the location in the currently active Shader.
			virtual void sendToActiveShader() const = 0;

			// The available types.
			enum Type { INT, VECTOR_2I, VECTOR_3I, VECTOR_4I, FLOAT, VECTOR_2F, VECTOR_3F, VECTOR_4F, MATRIX_22F, MATRIX_33F, MATRIX_44F, TEXTURE_2D };

		private:
			// The GL location of the uniform within the shader.
			int const location;

			// The type of the variable.
			int const type;
		};

		// An OpenGL uniform used by Shader and Material.
		class UniformInt : public Uniform
		{
		public:
			// Constructor.
			UniformInt(int location);

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
			UniformVector2i(int location);

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
			UniformVector3i(int location);

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
			UniformVector4i(int location);

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
			UniformFloat(int location);

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
			UniformVector2f(int location);

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
			UniformVector3f(int location);

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
			UniformVector4f(int location);

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
			UniformMatrix22f(int location);

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
			UniformMatrix33f(int location);

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
			UniformMatrix44f(int location);

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
			UniformTexture2d(int location, int textureSlot);

			// Send the value to the location in the currently active Shader.
			void sendToActiveShader() const override;

			// The texture slot that will be used.
			int const textureSlot;

			// The texture that will be sampled by the sampler.
			UsePtr<Texture> texture;
		};
	}
}
