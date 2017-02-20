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

			Ptr<Scene> createScene();

			void destroyScene(Ptr<Scene> const & scene);

			Ptr<ImageTarget> createImageTarget();

			void destroyImageTarget(Ptr<ImageTarget> const & target);

			void preRenderUpdate();

		private:
			PtrSet<Scene> scenes;
			PtrSet<ImageTarget> imageTargets;
		};
	}
}