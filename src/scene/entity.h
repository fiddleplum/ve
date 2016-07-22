#pragma once

#include "../vector.h"
#include "../matrix.h"
#include "../quaternion.h"

namespace ve
{
	namespace scene
	{
		class Entity
		{
		public:
			Entity();

			Vector3f const & getPosition() const;

			void setPosition(Vector3f position);

			Quaternionf const & getOrientation() const;

			void setOrientation(Quaternionf orientation);

			Matrix44f const & getLocalToWorldTransform() const;

			Matrix44f const & getWorldToLocalTransform() const;

		private:
			void checkTransforms() const;

			Vector3f position;
			Quaternionf orientation;
			mutable bool transformsNeedUpdate;
			mutable Matrix44f localToWorldTransform;
			mutable Matrix44f worldToLocalTransform;
		};
	}
}

