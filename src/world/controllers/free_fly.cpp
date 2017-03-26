#include <world/controllers/free_fly.hpp>

namespace ve
{
	namespace world
	{
		void FreeFlyController::setEntity(ve::Ptr<ve::world::Entity> const & entity_)
		{
			entity = entity_;
		}

		void FreeFlyController::handleInputEvent(ve::InputEvent const & inputEvent)
		{
			if (!entity.isValid())
			{
				return;
			}
			float speed = 20.f;
			if (inputEvent.getDevice() == ve::DeviceKeyboard)
			{
				float direction = (inputEvent.isPressed() ? 1.f : -1.f);
				switch (inputEvent.getAxis())
				{
					case ve::KeyboardA:
						acceleration[0] -= speed * direction; break;
					case ve::KeyboardD:
						acceleration[0] += speed * direction; break;
					case ve::KeyboardS:
						acceleration[1] -= speed * direction; break;
					case ve::KeyboardW:
						acceleration[1] += speed * direction; break;
					case ve::KeyboardQ:
						acceleration[2] -= speed * direction; break;
					case ve::KeyboardE:
						acceleration[2] += speed * direction; break;
				}
			}
			else if (inputEvent.getDevice() == ve::DeviceMouse)
			{
				switch (inputEvent.getAxis())
				{
					case ve::MouseX:
						if (rotating)
						{
							entity->setOrientation(entity->getOrientation() * ve::Quaternionf(-(float)inputEvent.getValue() / 100.f, ve::Vector3f {0.f, 0.f, 1.f}, true));
						} break;
					case ve::MouseY:
						if (rotating)
						{
							entity->setOrientation(entity->getOrientation() * ve::Quaternionf(-(float)inputEvent.getValue() / 100.f, ve::Vector3f {1.f, 0.f, 0.f}, true));
						} break;
					case ve::MouseRight:
						rotating = inputEvent.isPressed(); break;
				}
			}
		}

		void FreeFlyController::update(float secondsPerFrame)
		{
			if (!entity.isValid())
			{
				return;
			}
			entity->setPosition(entity->getPosition() + entity->getOrientation().rotate(velocity) * secondsPerFrame);
			velocity = acceleration;
			//velocity = std::powf(1e-3f, secondsPerFrame) * velocity + acceleration * secondsPerFrame;
			//entity->velocity = velocity;
		}
	}
}
