#pragma once

#include "world/entity.hpp"
#include "render/scene.hpp"

namespace ve
{
	namespace world
	{
		class Object final : public Entity
		{
		public:
			// Constructs an object with an empty model.
			Object(Ptr<render::Scene> const & scene);

			// Constructs an object from a filename.
			Object(Ptr<render::Scene> const & scene, std::string const & filename);

			// Destructs the object.
			virtual ~Object();

			// Returns the render model used by the object.
			Ptr<render::Model> getModel() const;

		private:
			void updateShader();

			Ptr<render::Scene> scene;
			Ptr<render::Model> model;
			OwnPtr<render::Mesh> mesh;
			int localToWorldTransformLocation;
		};
	}
}