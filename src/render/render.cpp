#include "render/render.hpp"

namespace ve
{
	namespace render
	{
		Render::Render()
		{

		}

		Render::~Render()
		{
			imageTargets.queueAllForErase();
			imageTargets.processEraseQueue();
			scenes.queueAllForErase();
			scenes.processEraseQueue();
		}

		Ptr<Scene> Render::createScene()
		{
			return *scenes.insertNew<Scene>();
		}

		void Render::destroyScene(Ptr<Scene> const & scene)
		{
			scenes.queueForErase(scene);
		}

		Ptr<ImageTarget> Render::createImageTarget()
		{
			return *imageTargets.insertNew<ImageTarget>();
		}

		void Render::destroyImageTarget(Ptr<ImageTarget> const & target)
		{
			imageTargets.queueForErase(target);
		}

		void Render::preRenderUpdate()
		{
			scenes.processEraseQueue();
			imageTargets.processEraseQueue();

			for (auto && imageTarget : imageTargets)
			{
				imageTarget->clearRenderedThisFrameFlag();
			}
		}
	}
}