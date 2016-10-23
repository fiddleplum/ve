#pragma once

#include "../ptr.h"
#include "../shader.h"
#include "../vertex_buffer_object.h"
#include "../texture.h"
#include <string>
#include <vector>

namespace ve
{
	namespace scene
	{
		class Model
		{
		public:
			Model();

			Model(std::string const & filename);

			void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs);

			void setVertices(void const * vertices, unsigned int numBytes);

			void setNumIndicesPerPrimitive(unsigned int num);

			void setIndices(unsigned int const * indices, unsigned int numIndices);

			void addTexture(UsePtr<Texture> texture, std::string const & type, unsigned int uvIndex);

			void addTextureFromFile(std::string const & filename, std::string const & type, unsigned int uvIndex);

			void clearTextures();

			void setColor(Vector3f const & emitColor, Vector4f const & diffuseColor);

			void setSpecular(unsigned int level, float strength);

			float getScale() const;

			void setScale(float scale);

			void render(Matrix44f const & projectionTransform, Matrix44f const & localToCameraTransform, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const;

			bool operator < (Model const & model) const;

			static const unsigned int maxLights = 4;

		private:
			struct TextureInfo
			{
				UsePtr<Texture> texture;
				std::string type;
				int samplerLocation;
				int uvIndex;
			};

			void updateShader();
			void updateShader120();
			void updateShader150();

			Vector3f emitColor;
			int emitColorLocation;
			Vector4f diffuseColor;
			int diffuseColorLocation;
			unsigned int specularLevel;
			int specularLevelLocation;
			float specularStrength;
			int specularStrengthLocation;

			int lightPositionsLocation;
			int lightColorsLocation;

			std::vector<TextureInfo> textureInfos;

			bool vertexHasNormal;
			bool vertexHasTangent;
			bool vertexHasColor;
			unsigned int numVertexUVs;
			unsigned int numBytesPerVertex;
			UsePtr<VertexBufferObject> vertexBufferObject;

			UsePtr<Shader> shader;
			bool shaderDirty;

			int projectionLocation;
			int worldViewLocation;

			float scale;
			int scaleLocation;
		};
	}
}

