#include <world/controller.hpp>
#include <world/entity.hpp>
#include <util/ptr.hpp>

namespace ve
{
	namespace world
	{
		class FreeFlyController : public ve::world::Controller
		{
		public:
			void setEntity(ve::Ptr<ve::world::Entity> const & entity_);

			void handleInputEvent(ve::InputEvent const & inputEvent);

			void update(float secondsPerFrame);

		private:
			ve::Ptr<ve::world::Entity> entity;
			ve::Vector3f acceleration;
			ve::Vector3f velocity;
			bool rotating = false;
		};
	}
}
