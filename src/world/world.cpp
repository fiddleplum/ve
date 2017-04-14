#include "world/world.hpp"

namespace ve
{
	namespace world
	{
		World::World(Ptr<render::Render> const & render)
		{
			scene = render->scenes.create();
			scene->setUniformsFunction([this](Ptr<render::Shader> const & shader)
			{
				unsigned int lightIndex = 0;
				for (auto && light : lights)
				{
					shader->setUniformValue(("light" + std::to_string(lightIndex) + "Position").c_str(), light->getPosition());
					shader->setUniformValue(("light" + std::to_string(lightIndex) + "Color").c_str(), light->getColor());
					lightIndex++;
				}
			});
		}

		World::~World()
		{
		}

		void World::setupTarget(Ptr<render::Target> const & target, Ptr<Camera> const & camera)
		{
			target->setScene(scene);
			target->setUniformsFunction([camera](Ptr<render::Shader> const & shader)
			{
				shader->setUniformValue("worldToCameraTramsform", camera->getWorldToLocalTransform());
				shader->setUniformValue("cameraToNdcTransform", camera->getLocalToNdcTransform());
			});
		}

		Ptr<render::Scene> World::getScene() const
		{
			return scene;
		}

		void World::destroyCamera(Ptr<Camera> const & camera)
		{
			cameras.queueForErase(camera);
		}

		void World::destroyLight(Ptr<Light> const & light)
		{
			lights.queueForErase(light);
		}

		void World::destroyObject(Ptr<Object> const & object)
		{
			objects.queueForErase(object);
		}

		void World::destroyController(Ptr<Controller> const & controller)
		{
			controllers.queueForErase(controller);
		}

		void World::update(float dt)
		{
			cameras.processEraseQueue();
			lights.processEraseQueue();
			objects.processEraseQueue();
			controllers.processEraseQueue();

			for (auto && controller : controllers)
			{
				controller->update(dt);
			}
		}

		void World::handleInputEvent(InputEvent const & inputEvent)
		{
			for (auto && controller : controllers)
			{
				controller->handleInputEvent(inputEvent);
			}
		}
	}
}