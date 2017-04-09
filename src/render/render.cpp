#include "render/render.hpp"

namespace ve
{
	namespace render
	{
		void Render::preRenderUpdate()
		{
			imageTargets.clean();

			for (auto && imageTarget : imageTargets)
			{
				imageTarget->clearRenderedThisFrameFlag();
			}
		}
	}
}