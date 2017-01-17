#pragma once

#include "util/vector.hpp"
#include "util/matrix.hpp"
#include "util/quaternion.hpp"

namespace ve
{
	namespace world
	{
		class Entity
		{
		public:
			// Constructs at the origin with the identity orientation.
			Entity();

			// Returns the position.
			Vector3f const & getPosition() const;

			// Sets the position.
			void setPosition(Vector3f const & position);

			// Returns the orientation.
			Quaternionf const & getOrientation() const;

			// Sets the orientation.
			void setOrientation(Quaternionf const & orientation);

			// Returns the local to world transform matrix.
			Matrix44f const & getLocalToWorldTransform() const;

			// Returns the world to local transform matrix.
			Matrix44f const & getWorldToLocalTransform() const;

		protected:
			// Virtual destructor for inheritance.
			virtual ~Entity();

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

