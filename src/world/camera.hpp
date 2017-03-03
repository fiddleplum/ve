#pragma once

#include "world/entity.hpp"
#include "render/scene.hpp"
#include "util/ray.hpp"

namespace ve
{
	namespace world
	{
		class Camera : public Entity
		{
		public:
			// Constructs a camera in perspective mode, with an aspect ratio of 1, a near of .1, a far of 1000, an an fovY of 90 degrees.
			Camera();

			// Destructs the camera.
			virtual ~Camera();

			// Returns the aspect ratio.
			float getAspectRatio() const;

			// Sets the aspect ratio.
			void setAspectRatio(float aspectRatio);

			// Returns the near z distance.
			float getNear() const;

			// Sets the near distance.
			void setNear(float near);

			// Returns the far z distance.
			float getFar() const;

			// Sets the far distance.
			void setFar(float far);

			// Returns the the maximum field of view, if in perspective mode.
			float getFov() const;

			// Sets the maximum field of view and sets the camera into perspective mode.
			void setFov(float fovY);

			// Returns the maximum view size, if in orthogonal mode.
			float getSize() const;

			// Sets the maximum view size and sets the camera into orthogonal mode.
			void setSize(float size);

			// Returns the normalized device coordinates of a given position in world coordinates.
			Vector2f getNdcPosition(Vector3f worldPosition) const;

			// Returns a ray in world coordinates of the given position in normalized device coordinates.
			Ray3f getRay(Vector2f ndcPosition) const;

			// Returns the local to normalized device coordinates transform matrix.
			Matrix44f const & getLocalToNdcTransform() const;

			// Returns the normalized device coordinates to local transform matrix.
			Matrix44f const & getNdcToLocalTransform() const;

		private:
			void checkTransforms() const;

			float aspectRatio;
			float near;
			float far;
			float fov;
			float size;
			bool perspective;
			mutable bool transformsNeedUpdate;
			mutable Matrix44f localToNdcTransform;
			mutable Matrix44f ndcToLocalTransform;
		};
	}
}

