#pragma once

#include "element.h"
#include "../ptr.h"
#include "../scene/camera.h"
#include "../scene/scene.h"

namespace ve
{
	namespace gui
	{
		class Viewport : public Element
		{
		public:
			// Returns the camera used by the viewport.
			UsePtr<scene::Camera> getCamera() const;

			// Returns the scene to be rendered by the viewport.
			UsePtr<scene::Scene> getScene() const;

			// Sets the scene to be rendered and the camera within that scene to be used.
			void setSceneAndCamera(UsePtr<scene::Scene> scene, UsePtr<scene::Camera> camera);

		protected:
			void handleNewBounds() override;

			void render(Vector2i windowSize) const override;

		private:
			void updateCamera();

			UsePtr<scene::Scene> scene;
			UsePtr<scene::Camera> camera;
		};
	}
}

