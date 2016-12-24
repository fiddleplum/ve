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
			Object(Ptr<Scene> scene);

			~Object();

		private:
			Ptr<Scene> scene;
			Ptr<Model> model;
			OwnPtr<VertexBufferObject> vbo;
			int localToWorldTransformLocation;
		};
	}
}