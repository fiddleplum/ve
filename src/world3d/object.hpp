#pragma once

#include "entity.hpp"
#include "../render/scene.hpp"

namespace ve
{
	namespace world3d
	{
		class Object : public Entity
		{
		public:
			Object(Ptr<Scene> const & scene);

			Object(Ptr<Scene> const & scene, std::string const & filename);

			~Object();

			Ptr<Model> getModel() const;

		private:
			void updateShader();

			Ptr<Scene> scene;
			Ptr<Model> model;
			OwnPtr<VertexBufferObject> vbo;
			int localToWorldTransformLocation;
		};
	}
}