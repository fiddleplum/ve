#include "world/camera.hpp"
#include "util/math.hpp"

namespace ve
{
	namespace world
	{
		Camera::Camera()
		{
			aspectRatio = 1.0f;
			near = 0.1f;
			far = 1000.0f;
			fov = (float)math::PI_OVER_2;
			size = 1.0f;
			perspective = true;
			localToNdcTransform = ndcToLocalTransform = Matrix44f::identity();
			localToNdcTransform(1, 1) = 0;
			localToNdcTransform(2, 2) = 0;
			localToNdcTransform(3, 1) = 1;
			localToNdcTransform(3, 3) = 0;
			ndcToLocalTransform(1, 1) = 0;
			ndcToLocalTransform(1, 3) = 1;
			ndcToLocalTransform(2, 2) = 0;
			transformsNeedUpdate = true;
		}

		Camera:: ~Camera()
		{

		}

		float Camera::getAspectRatio() const
		{
			return aspectRatio;
		}

		void Camera::setAspectRatio(float aspectRatio_)
		{
			aspectRatio = aspectRatio_;
			transformsNeedUpdate = true;
		}

		float Camera::getNear() const
		{
			return near;
		}

		void Camera::setNear(float near_)
		{
			near = near_;
			transformsNeedUpdate = true;
		}

		float Camera::getFar() const
		{
			return far;
		}

		void Camera::setFar(float far_)
		{
			far = far_;
			transformsNeedUpdate = true;
		}

		float Camera::getFov() const
		{
			return perspective ? fov : 0;
		}

		void Camera::setFov(float fov_)
		{
			fov = fov_;
			perspective = true;
			transformsNeedUpdate = true;
		}

		float Camera::getSize() const
		{
			return !perspective ? size : 0;
		}

		void Camera::setSize(float size_)
		{
			size = size_;
			perspective = false;
			transformsNeedUpdate = true;
		}

		Vector2f Camera::getNdcPosition(Vector3f worldPosition) const
		{
			checkTransforms();
			Vector3f localPosition = getWorldToLocalTransform().transform(worldPosition, 1);
			return localToNdcTransform.transform(localPosition, 1).shrink<2>();
		}

		Ray3f Camera::getRay(Vector2f ndcPosition) const
		{
			checkTransforms();
			Vector3f localPosition = ndcToLocalTransform.transform(Vector3f({ndcPosition[0], +1.f, ndcPosition[1]}), 1);
			return Ray3f(getPosition(), getLocalToWorldTransform().transform(localPosition, 1));
		}

		Matrix44f const & Camera::getLocalToNdcTransform() const
		{
			checkTransforms();
			return localToNdcTransform;
		}

		Matrix44f const & Camera::getNdcToLocalTransform() const
		{
			checkTransforms();
			return ndcToLocalTransform;
		}

		void Camera::checkTransforms() const
		{
			if (transformsNeedUpdate)
			{
				float scale;
				if (perspective)
				{
					scale = std::tan(fov / 2.0f);
				}
				else
				{
					scale = size;
				}
				if (scale == 0 || aspectRatio == 0 || far == near || near == 0 || far == 0)
				{
					return;
				}
				float scaleInv = 1.0f / scale;
				if (aspectRatio >= 1)
				{
					localToNdcTransform(0, 0) = scaleInv / aspectRatio;
					localToNdcTransform(1, 2) = scaleInv;
					ndcToLocalTransform(0, 0) = scale * aspectRatio;
					ndcToLocalTransform(2, 1) = scale;
				}
				else
				{
					localToNdcTransform(0, 0) = scaleInv;
					localToNdcTransform(1, 2) = scaleInv * aspectRatio;
					ndcToLocalTransform(0, 0) = scale;
					ndcToLocalTransform(2, 1) = scale / aspectRatio;
				}
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
				transformsNeedUpdate = false;
			}
		}
	}
}

