#pragma once

//#include "object.h"
#include "light.h"
#include "camera.h"
#include "../event.h"
#include "../ptr.h"
//#include "ptr_set.h"
#include <functional>
#include <set>

namespace ve
{
	namespace scene
	{
		class Scene
		{
		public:
			Scene();

			UsePtr<Light> addLight();

			void removeLight(UsePtr<Light>);

			UsePtr<Camera> addCamera();

			void removeCamera(UsePtr<Camera> camera);

			//UsePtr<SceneObject> addObject();

			//void removeObject(Ptr<SceneObject> object);

			void setInputEventHandler(std::function<void(InputEvent const &)> handler);

			void setUpdateHandler(std::function<void(float)> handler);

			void setPreRenderUpdateHandler(std::function<void()> handler);

			// Called by app to handle an event.
			void handleInputEvent(InputEvent const & inputEvent);

			// Called by App to update the scene every frame.
			void update(float dt);

			// Called by the Viewport to update the scene after a regular update but before the render. Good for things that keep track of other scene elements.
			void preRenderUpdate();

			// Called by GuiViewport to render the scene.
			void render(UsePtr<Camera> camera);

		private:
			std::set<OwnPtr<Light>> lights;
			std::set<OwnPtr<Camera>> cameras;
			//std::set<OwnPtr<Object>> objects;
			std::function<void(InputEvent const &)> inputEventHandler;
			std::function<void(float)> updateHandler;
			std::function<void()> preRenderUpdateHandler;
		};
	}
}

