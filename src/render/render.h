#pragma once

#include "shader.h"
#include "texture.h"
#include "material.h"
#include "vertex_buffer_object.h"
#include "model.h"
#include "stage.h"

namespace ve
{
	namespace render
	{
		class Render
		{
		public:
			UsePtr<Stage> addNewStage();

			void removeStage(UsePtr<Stage> stage);

			void render() const;

			Cache<Shader> & shaderCache();

			Cache<Texture> & textureCache();

			Cache<Material> & materialCache();

			Cache<VertexBufferObject> & vertexBufferObjectCache();

			Cache<Model> & modelCache();

		private:
			std::vector<OwnPtr<Stage>> stages;

			Cache<Shader> shaders;
			Cache<Texture> textures;
			Cache<Material> materials;
			Cache<VertexBufferObject> vertexBufferObjects;
			Cache<Model> models;
		};
	}
}

