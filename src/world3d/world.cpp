#include "world.hpp"

namespace ve
{
	namespace world3d
	{
		World::World()
		{
			scene.setNew();
			scene->addUniformsFunction((int)(intptr_t)this, [this](Ptr<Shader> const & shader)
			{
				// How do I set the currently rendering camera?
				// Where is the active camera stored?
			});
		}

		Ptr<Scene> World::getScene() const
		{
			return scene;
		}

		Ptr<Camera> World::createCamera()
		{
			auto camera = OwnPtr<Camera>::returnNew(scene);
			cameras.insert(camera);
			return camera;
		}

		void World::destroyCamera(Ptr<Camera> const & camera)
		{
			auto it = std::find(cameras.begin(), cameras.end(), camera);
			if (it != cameras.end())
			{
				throw std::runtime_error("Camera not found. ");
			}
			else
			{
				cameras.erase(it);
			}
		}

		Ptr<Light> World::createLight()
		{
			auto light = OwnPtr<Light>::returnNew();
			lights.insert(light);
			return light;
		}

		void World::destroyLight(Ptr<Light> const & light)
		{
			auto it = std::find(lights.begin(), lights.end(), light);
			if (it != lights.end())
			{
				throw std::runtime_error("Light not found. ");
			}
			else
			{
				lights.erase(it);
			}
		}

		Ptr<Object> World::createObject()
		{
			auto object = OwnPtr<Object>::returnNew(scene);
			objects.insert(object);
			return object;
		}

		void World::destroyobject(Ptr<Object> const & object)
		{
			auto it = std::find(objects.begin(), objects.end(), object);
			if (it != objects.end())
			{
				throw std::runtime_error("Object not found. ");
			}
			else
			{
				objects.erase(it);
			}
		}
	}
}