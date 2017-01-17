#pragma once

#include "world/entity.hpp"
#include "render/scene.hpp"

namespace ve
{
	namespace world
	{
		class Object : public Entity
		{
		public:
			Object(Ptr<render::Scene> const & scene);

			Object(Ptr<render::Scene> const & scene, std::string const & filename);

			~Object();

			Ptr<render::Model> getModel() const;

		private:
			void updateShader();

			Ptr<render::Scene> scene;
			Ptr<render::Model> model;
			OwnPtr<render::VertexBufferObject> vbo;
			int localToWorldTransformLocation;
		};
	}
}