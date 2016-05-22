#include "camera.h"

namespace ve
{
	namespace scene
	{
		Camera::Camera()
		{
			aspectRatio = 1.0f;
			near = 0.1f;
			far = 1000.0f;
			fovY = 3.14159f / 2.0f;
			height = 1.0f;
			perspective = true;
			localToNdcTransform = ndcToLocalTransform = Matrix44f::identity();
			localToNdcTransform(1, 1) = 0;
			localToNdcTransform(2, 2) = 0;
			localToNdcTransform(3, 1) = 1;
			localToNdcTransform(3, 3) = 0;
			ndcToLocalTransform(1, 1) = 0;
			ndcToLocalTransform(1, 3) = 1;
			ndcToLocalTransform(2, 2) = 0;
			localToNdcTransformNeedsUpdate = true;
		}

		float Camera::getAspectRatio() const
		{
			return aspectRatio;
		}

		void Camera::setAspectRatio(float newAspectRatio)
		{
			aspectRatio = newAspectRatio;
			localToNdcTransformNeedsUpdate = true;
		}

		float Camera::getNear() const
		{
			return near;
		}

		void Camera::setNear(float newNear)
		{
			near = newNear;
			localToNdcTransformNeedsUpdate = true;
		}

		float Camera::getFar() const
		{
			return far;
		}

		void Camera::setFar(float newFar)
		{
			far = newFar;
			localToNdcTransformNeedsUpdate = true;
		}

		float Camera::getFovY() const
		{
			return fovY;
		}

		void Camera::setFovY(float newFovY)
		{
			fovY = newFovY;
			perspective = true;
			localToNdcTransformNeedsUpdate = true;
		}

		void Camera::setHeight(float newHeight)
		{
			height = newHeight;
			perspective = false;
			localToNdcTransformNeedsUpdate = true;
		}

		Vector2f Camera::getNdcPosition(Vector3f worldPosition) const
		{
			if (localToNdcTransformNeedsUpdate)
			{
				const_cast<Camera *>(this)->updateLocalToNdc();
			}
			Vector3f localPosition = getWorldToLocalTransform().transform(worldPosition, 1);
			return localToNdcTransform.transform(localPosition, 1).shrink<2>();
		}

		Ray3f Camera::getRay(Vector2f ndcPosition) const
		{
			if (localToNdcTransformNeedsUpdate)
			{
				const_cast<Camera *>(this)->updateLocalToNdc();
			}
			Vector3f localPosition = ndcToLocalTransform.transform(Vector3f({ ndcPosition[0], +1.f, ndcPosition[1] }), 1);
			return Ray3f(getPosition(), getLocalToWorldTransform().transform(localPosition, 1));
		}

		Matrix44f const & Camera::getLocalToNdcTransform() const
		{
			if (localToNdcTransformNeedsUpdate)
			{
				const_cast<Camera *>(this)->updateLocalToNdc();
			}
			return localToNdcTransform;
		}

		Matrix44f const & Camera::getNdcToLocalTransform() const
		{
			if (localToNdcTransformNeedsUpdate)
			{
				const_cast<Camera *>(this)->updateLocalToNdc();
			}
			return ndcToLocalTransform;
		}

		void Camera::updateLocalToNdc()
		{
			float scale;
			if (perspective)
			{
				scale = std::tan(fovY / 2.0f);
			}
			else
			{
				scale = height;
			}
			if (scale == 0 || aspectRatio == 0 || far == near || near == 0 || far == 0)
			{
				return;
			}
			float scaleInv = 1.0f / scale;
			localToNdcTransform(0, 0) = scaleInv / aspectRatio;
			localToNdcTransform(1, 2) = scaleInv;
			ndcToLocalTransform(0, 0) = scale * aspectRatio;
			ndcToLocalTransform(2, 1) = scale;
			if (perspective)
			{
				float nf2 = 2 * near * far;
				float nmf = near - far;
				float npf = near + far;
				localToNdcTransform(2, 1) = -npf / nmf;
				localToNdcTransform(2, 3) = nf2 / nmf;
				ndcToLocalTransform(3, 2) = nmf / nf2;
				ndcToLocalTransform(3, 3) = npf / nf2;
			}
			else
			{
				// TODO, NEEDS FIXING
				float nmf = near - far;
				float npf = near + far;
				localToNdcTransform(2, 1) = 2 / nmf;
				localToNdcTransform(2, 3) = -npf / nmf;
				ndcToLocalTransform(1, 2) = nmf / 2;
				ndcToLocalTransform(2, 3) = npf / 2;
			}
			localToNdcTransformNeedsUpdate = false;
		}
	}
}

