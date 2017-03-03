#pragma once

#include "world/camera.hpp"
#include "world/light.hpp"
#include "world/object.hpp"
#include "world/controller.hpp"
#include "render/scene.hpp"
#include "render/target.hpp"
#include "util/ptr_set.hpp"

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

			void destroyObject(Ptr<Object> const & object);

			template <typename ControllerType>
			Ptr<ControllerType> createController();

			void destroyController(Ptr<Controller> const & controller);

			void update(float dt);

			void handleInputEvent(InputEvent const & inputEvent);

		private:
			OwnPtr<render::Scene> scene;
			PtrSet<Camera> cameras;
			PtrSet<Light> lights;
			PtrSet<Object> objects;
			PtrSet<Controller> controllers;
		};

		template <typename CameraType>
		Ptr<CameraType> World::createCamera()
		{
			static_assert(std::is_base_of<Camera, CameraType>::value, "Class is not derived from Camera. ");
			return cameras.insertNew<CameraType>();
		}

		template <typename LightType>
		Ptr<LightType> World::createLight()
		{
			static_assert(std::is_base_of<Light, LightType>::value, "Class is not derived from Light. ");
			return lights.insertNew<LightType>();
		}

		template <typename ObjectType>
		Ptr<ObjectType> World::createObject()
		{
			static_assert(std::is_base_of<Object, ObjectType>::value, "Class is not derived from Object. ");
			return objects.insertNew<ObjectType>(scene);
		}

		template <typename ControllerType>
		Ptr<ControllerType> World::createController()
		{
			static_assert(std::is_base_of<Controller, ControllerType>::value, "Class is not derived from Controller. ");
			return controller.insertNew<ControllerType>();
		}
	}
}