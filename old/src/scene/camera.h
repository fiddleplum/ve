#pragma once

#include "entity.h"
#include "../ray.h"

namespace ve
{
	namespace scene
	{
		class Camera : public Entity
		{
		public:
			Camera();

			float getAspectRatio() const;

			void setAspectRatio(float aspectRatio);

			float getNear() const;

			void setNear(float near);

			float getFar() const;

			void setFar(float far);

			float getFovY() const;

			void setFovY(float fovY); // also sets the camera to perspective mode

			float getHeight() const;

			void setHeight(float height); // also sets the camera to orthogonal mode

			Vector2f getNdcPosition(Vector3f worldPosition) const;

			Ray3f getRay(Vector2f ndcPosition) const;

			Matrix44f const & getLocalToNdcTransform() const;

			Matrix44f const & getNdcToLocalTransform() const;

		private:
			void checkTransforms() const;

			float aspectRatio;
			float near;
			float far;
			float fovY;
			float height;
			bool perspective;
			mutable bool transformsNeedUpdate;
			mutable Matrix44f localToNdcTransform;
			mutable Matrix44f ndcToLocalTransform;
		};
	}
}

