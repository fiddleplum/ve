#pragma once

#include "world/camera.hpp"
#include "world/light.hpp"
#include "world/object.hpp"
#include "render/scene.hpp"
#include "render/target.hpp"

namespace ve
{
	namespace world
	{
		class World final
		{
		public:
			World();

			void setupTarget(Ptr<render::Target> const & target, Ptr<Camera> const & camera);

			Ptr<render::Scene> getScene() const;

			Ptr<Camera> createCamera();

			void destroyCamera(Ptr<Camera> const & camera);

			Ptr<Light> createLight();

			void destroyLight(Ptr<Light> const & light);

			Ptr<Object> createObject();

			void destroyobject(Ptr<Object> const & object);

		private:
			OwnPtr<render::Scene> scene;
			std::unordered_set<OwnPtr<Camera>> cameras;
			std::unordered_set<OwnPtr<Light>> lights;
			std::unordered_set<OwnPtr<Object>> objects;
		};
	}
}