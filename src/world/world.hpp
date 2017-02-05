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
		class World
		{
		public:
			// Constructs an empty world. Fill it with things!
			World();

			// Destructs the world.
			virtual ~World();

			void setupTarget(Ptr<render::Target> const & target, Ptr<Camera> const & camera);

			Ptr<render::Scene> getScene() const;

			template <typename CameraType>
			Ptr<CameraType> createCamera();

			void destroyCamera(Ptr<Camera> const & camera);

			template <typename LightType>
			Ptr<LightType> createLight();

			void destroyLight(Ptr<Light> const & light);

			template <typename ObjectType>
			Ptr<ObjectType> createObject();

			void destroyobject(Ptr<Object> const & object);

		private:
			void update(float dt);

			OwnPtr<render::Scene> scene;
			std::unordered_set<OwnPtr<Camera>> cameras;
			std::unordered_set<OwnPtr<Light>> lights;
			std::unordered_set<OwnPtr<Object>> objects;

			friend class App;
		};

		template <typename CameraType>
		Ptr<CameraType> World::createCamera()
		{
			static_assert(std::is_base_of<Camera, CameraType>::value, "Class is not derived from Camera. ");
			OwnPtr<Camera> camera = OwnPtr<CameraType>::returnNew();
			cameras.insert(camera);
			return camera;
		}

		template <typename LightType>
		Ptr<LightType> World::createLight()
		{
			static_assert(std::is_base_of<Light, LightType>::value, "Class is not derived from Light. ");
			OwnPtr<Light> light = OwnPtr<LightType>::returnNew();
			lights.insert(light);
			return light;
		}

		template <typename ObjectType>
		Ptr<ObjectType> World::createObject()
		{
			static_assert(std::is_base_of<Object, ObjectType>::value, "Class is not derived from Object. ");
			OwnPtr<Object> object = OwnPtr<ObjectType>::returnNew(scene);
			objects.insert(object);
			return object;
		}
	}
}