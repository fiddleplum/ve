#include "viewport.h"
#include "../render/open_gl.h"

namespace ve
{
	namespace gui
	{
		UsePtr<scene::Camera> Viewport::getCamera() const
		{
			return camera;
		}

		UsePtr<scene::Scene> Viewport::getScene() const
		{
			return scene;
		}

		void Viewport::setSceneAndCamera(UsePtr<scene::Scene> scene_, UsePtr<scene::Camera> camera_)
		{
			camera = camera_;
			scene = scene_;
			updateCamera();
		}

		void Viewport::handleNewBounds()
		{
			updateCamera();
		}

		void Viewport::render(Vector2i windowSize) const
		{
			if (scene && camera)
			{
				Recti bounds = getBounds();
				glViewport(bounds.min[0], windowSize[1] - (bounds.max[1] + 1), (bounds.max[0] + 1) - bounds.min[0], (bounds.max[1] + 1) - bounds.min[1]);
				scene->render(camera);
				glViewport(0, 0, windowSize[0], windowSize[1]);
			}
		}

		void Viewport::updateCamera()
		{
			if (camera)
			{
				Vector2i size = getBounds().getSize();
				if (size[1] != 0)
					camera->setAspectRatio((float)size[0] / (float)size[1]);
			}
		}
	}
}

