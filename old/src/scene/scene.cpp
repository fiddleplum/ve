#include "scene.h"
#include "../render/open_gl.h"
#include <map>
#include <vector>

namespace ve
{
	namespace scene
	{
		Scene::Scene()
		{
		}

		UsePtr<Light> Scene::addLight()
		{
			return *lights.insert(OwnPtr<Light>::createNew()).first;
		}

		void Scene::removeLight(UsePtr<Light> light)
		{
			auto iter = std::find(lights.begin(), lights.end(), light);
			if (iter == lights.end())
			{
				throw std::exception("Light not in scene. ");
			}
			lights.erase(iter);
		}

		UsePtr<Camera> Scene::addCamera()
		{
			return *cameras.insert(OwnPtr<Camera>::createNew()).first;
		}

		void Scene::removeCamera(UsePtr<Camera> camera)
		{
			auto iter = std::find(cameras.begin(), cameras.end(), camera);
			if (iter == cameras.end())
			{
				throw std::exception("Camera not in scene. ");
			}
			cameras.erase(iter);
		}

		UsePtr<Object> Scene::addObject()
		{
			return *objects.insert(OwnPtr<Object>::createNew()).first;
		}

		void Scene::removeObject(UsePtr<Object> object)
		{
			auto iter = std::find(objects.begin(), objects.end(), object);
			if (iter == objects.end())
			{
				throw std::exception("Object not in scene. ");
			}
			objects.erase(iter);
		}

		void Scene::setInputEventHandler(std::function<void(InputEvent const &)> handler)
		{
			this->inputEventHandler = handler;
		}

		void Scene::setUpdateHandler(std::function<void(float)> handler)
		{
			this->updateHandler = handler;
		}

		void Scene::setPreRenderUpdateHandler(std::function<void()> handler)
		{
			this->preRenderUpdateHandler = handler;
		}

		void Scene::handleInputEvent(InputEvent const & inputEvent)
		{
			if (inputEventHandler)
			{
				inputEventHandler(inputEvent);
			}
		}

		void Scene::update(float dt)
		{
			if (updateHandler)
			{
				updateHandler(dt);
			}
		}

		void Scene::preRenderUpdate()
		{
			if (preRenderUpdateHandler)
			{
				preRenderUpdateHandler();
			}
		}

		void Scene::render(UsePtr<Camera> camera)
		{
			// Set the OpenGL settings.
			glEnable(GL_DEPTH_TEST);

			// Create the list of models to render. O(n log n)
			for (auto object : objects)
			{
			}
			//std::multimap<UsePtr<SceneModel>, UsePtr<SceneEntity>> modelsToRender;
			//for (auto object : objects)
			//{
			//	for (auto model : object->models)
			//	{
			//		modelsToRender.insert({UsePtr<SceneModel>(model), UsePtr<SceneEntity>(object)});
			//	}
			//}

			//// Prepare the lights.
			//std::vector<Coord3f> lightPositions;
			//std::vector<Coord3f> lightColors;
			//for (UsePtr<Light> light : lights)
			//{
			//	lightPositions.push_back(camera->getWorldToCameraTransform().transform(light->getPosition(), 1));
			//	lightColors.push_back(light->getColor());
			//}
			//if (!lights.empty())
			//{
			//	while (lightPositions.size() < SceneModel::maxLights)
			//	{
			//		lightPositions.push_back({0, 0, 0});
			//		lightColors.push_back({0, 0, 0});
			//	}
			//}

			//// Do the render.
			//for (auto pair : modelsToRender)
			//{
			//	pair.first->render(camera->getCameraToNdcTransform(), camera->getWorldToCameraTransform() * pair.second->getLocalToWorldTransform(), lightPositions, lightColors);
			//}

			glDisable(GL_DEPTH_TEST);
		}

		bool Scene::less(OwnPtr<Object> const & lhs, OwnPtr<Object> const & rhs)
		{
			UsePtr<render::Model> lhsModel = lhs->getModel();
			UsePtr<render::Model> rhsModel = rhs->getModel();
			UsePtr<render::Material> lhsMaterial = lhsModel->getMaterial();
			UsePtr<render::Material> rhsMaterial = rhsModel->getMaterial();
			if (lhsMaterial != rhsMaterial)
			{
				if (lhsMaterial->getShader() != rhsMaterial->getShader())
				{
					return lhsMaterial->getShader() < rhsMaterial->getShader();
				}
				else
				{
					return true;// compare textures
				}
			}
			else
			{
				return lhs < rhs;
			}
		}
	}
}

