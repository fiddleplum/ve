#pragma once

#include "object.h"
#include "light.h"
#include "camera.h"
#include "../event.h"
#include "../ptr.h"
#include <functional>
#include <set>

namespace ve
{
	void loop();

	namespace gui
	{
		class Viewport;
	}

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

			UsePtr<Object> addObject();

			void removeObject(UsePtr<Object> object);

			void setInputEventHandler(std::function<void(InputEvent const &)> handler);

			void setUpdateHandler(std::function<void(float)> handler);

			void setPreRenderUpdateHandler(std::function<void()> handler);

		protected:
			// Called by app to handle an event.
			void handleInputEvent(InputEvent const & inputEvent);

			// Called by app to update the scene every frame.
			void update(float dt);

			// Called by the Viewport to update the scene after a regular update but before the render. Good for things that keep track of other scene elements.
			void preRenderUpdate();

			// Called by Viewport to render the scene.
			void render(UsePtr<Camera> camera);

		private:
			static bool less(OwnPtr<Object> const &, OwnPtr<Object> const &);

			std::set<OwnPtr<Light>> lights;
			std::set<OwnPtr<Camera>> cameras;
			std::set<OwnPtr<Object>, bool(*)(OwnPtr<Object> const &, OwnPtr<Object> const &)> objects {less};
			std::function<void(InputEvent const &)> inputEventHandler;
			std::function<void(float)> updateHandler;
			std::function<void()> preRenderUpdateHandler;

			friend class gui::Viewport;
			friend void ve::loop();
		};
	}
}

