#include "render.h"

namespace ve
{
	namespace render
	{
		UsePtr<Stage> Render::addNewStage()
		{
			return UsePtr<Stage>();
		}

		void Render::removeStage(UsePtr<Stage> stage)
		{
		}

		void Render::render() const
		{
			for (auto stage : stages)
			{
				//stage->render();
			}
		}

		Cache<Shader> & Render::shaderCache()
		{
			return shaders;
		}

		Cache<Texture> & Render::textureCache()
		{
			return textures;
		}

		Cache<Material> & Render::materialCache()
		{
			return materials;
		}

		Cache<VertexBufferObject> & Render::vertexBufferObjectCache()
		{
			return vertexBufferObjects;
		}

		Cache<Model> & Render::modelCache()
		{
			return models;
		}
	}
}

