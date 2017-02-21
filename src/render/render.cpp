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
			imageTargets.processEraseQueue();

			for (auto && imageTarget : imageTargets)
			{
				imageTarget->clearRenderedThisFrameFlag();
			}
		}
	}
}