#include "render/target.hpp"
#include "render/scene.hpp"
#include "util/ptr_list.hpp"
#include "util/ptr_set.hpp"

namespace ve
{
	namespace render
	{
		class Render final
		{
		public:
			Render();

			~Render();

			Ptr<ImageTarget> createImageTarget();

			void destroyImageTarget(Ptr<ImageTarget> const & target);

			void preRenderUpdate();

		private:
			PtrSet<ImageTarget> imageTargets;
		};
	}
}