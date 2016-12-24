#pragma once

#include "camera.hpp"
#include "light.hpp"
#include "object.hpp"
#include "../render/scene.hpp"
#include "../render/stage.hpp"

namespace ve
{
	namespace world3d
	{
		class World final
		{
		public:
			World();

			void setupStage(Ptr<Stage> const & stage, Ptr<Camera> const & camera);

			Ptr<Scene> getScene() const;

			Ptr<Camera> createCamera();

			void destroyCamera(Ptr<Camera> const & camera);

			Ptr<Light> createLight();

			void destroyLight(Ptr<Light> const & light);

			Ptr<Object> createObject();

			void destroyobject(Ptr<Object> const & object);

		private:
			OwnPtr<Scene> scene;
			std::unordered_set<OwnPtr<Camera>> cameras;
			std::unordered_set<OwnPtr<Light>> lights;
			std::unordered_set<OwnPtr<Object>> objects;
		};
	}
}