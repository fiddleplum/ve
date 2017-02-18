#pragma once

#include "matrix.hpp"

namespace ve
{
	// This is a standard mathematical quaternion class. T is the type of its elements.
	template <typename T>
	class Quaternion
	{
	public:
		// Constructs it as the identity.
		Quaternion();

		// Constructs it with specific numbers.
		Quaternion(T r, T i, T j, T k);

		// Constructs it with a real and imaginary part.
		Quaternion(T r, Vector<3, T> ijk);

		// Constructs it with an angle and axis. If axisIsNormalized is true, the axis will not be normalized.
		Quaternion(T angle, Vector<3, T> const & axis, bool axisIsNormalized);

		// Constructs it as a rotation from start to end. If vectorsAreNormalized is true, start and end will not be normalized.
		Quaternion(Vector<3, T> const & start, Vector<3, T> const & end, bool vectorsAreNormalized);

		// Constructs it as a trio of euler angles.
		Quaternion(T yaw, T pitch, T roll);

		// Returns the conjugate.
		Quaternion<T> conjugate() const;

		// Returns the reciprocal.
		Quaternion<T> reciprocal() const;

		// Returns the norm.
		T norm() const;

		// Returns the square of the norm.
		T normSq() const;

		// Normalizes it.
		void normalize();

		// Returns the vector of the quaternion as a rotation operation applied to vector v. Assumes this is normalized.
		Vector<3, T> rotate(Vector<3, T> const & v) const;

		// Returns the 0, 1, or 2 axis of the frame represented by this. Assumes this is normalized.
		Vector<3, T> getAxis(unsigned int i) const;

		// Returns the rotation matrix represented by this, for pre-multiplying. Assumes this is normalized.
		Matrix<3, 3, T> getMatrix() const;

		// The real part.
		T r;

		// The imaginary part.
		Vector<3, T> ijk;
	};

	typedef Quaternion<float> Quaternionf;
	typedef Quaternion<double> Quaterniond;

	// Returns the addition of two quaternions.
	template <typename T> Quaternion<T> operator + (Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs);

	// Returns the subtraction of two quaternions.
	template <typename T> Quaternion<T> operator - (Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs);

	// Returns the multplication of two quaternions.
	template <typename T> Quaternion<T> operator * (Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs);

	// Returns the multiplication of a quaternion and a scalar.
	template <typename T> Quaternion<T> operator * (Quaternion<T> const & q, T t);

	// Returns the multiplication of a scalar and a quaternion.
	template <typename T> Quaternion<T> operator * (T t, Quaternion<T> const & q);

	// Template implementation

	template <typename T>
	Quaternion<T>::Quaternion()
	{
		r = 1;
		ijk = { 0, 0, 0 };
	}

	template <typename T>
	Quaternion<T>::Quaternion(T r_, T i, T j, T k)
	{
		r = r;
		ijk = { i, j, k };
	}

	template <typename T>
	Quaternion<T>::Quaternion(T r_, Vector<3, T> ijk_)
	{
		r = r_;
		ijk = ijk_;
	}

	template <typename T>
	Quaternion<T>::Quaternion(Vector<3, T> const & start, Vector<3, T> const & end, bool vectorsAreNormalized)
	{
		r = startVector.dot(end);
		Vector<3, T> axis;
		if (r != -1)
		{
			ijk = startVector.cross(end);
		}
		else
		{
			ijk = start.perpendicular();
		}
		if (vectorsAreNormalized)
		{
			r += 1;
		}
		else
		{
			r += start.norm() * end.norm();
		}
		normalize();
	}

	template <typename T>
	Quaternion<T>::Quaternion(T angle, Vector<3, T> const & axis, bool axisIsNormalized)
	{
		r = std::cos(angle / (T)2);
		if (axisIsNormalized)
		{
			ijk = axis;
		}
		else
		{
			ijk = axis.unit();
		}
		ijk *= std::sin(angle / (T)2);
	}

	template <typename T>
	Quaternion<T>::Quaternion(T yaw, T pitch, T roll)
	{
		// remember yaw goes positive to the right (if you're looking down +y)
		T cosYaw2 = cos(yaw / 2);
		T sinYaw2 = sin(yaw / 2);
		T cosPitch2 = cos(pitch / 2);
		T sinPitch2 = sin(pitch / 2);
		T cosRoll2 = cos(roll / 2);
		T sinRoll2 = sin(roll / 2);
		r = cosYaw2 * cosPitch2 * cosRoll2 - sinYaw2 * sinPitch2 * sinRoll2;
		ijk[1] = cosYaw2 * cosPitch2 * sinRoll2 + sinYaw2 * sinPitch2 * cosRoll2;
		ijk[0] = cosYaw2 * sinPitch2 * cosRoll2 - sinYaw2 * cosPitch2 * sinRoll2;
		ijk[2] = sinYaw2 * cosPitch2 * cosRoll2 + cosYaw2 * sinPitch2 * sinRoll2;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::conjugate() const
	{
		return Quaternion<T>(r, -ijk);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::reciprocal() const
	{
		T nSq = normSq();
		if (nSq == 0)
		{
			throw std::exception();
		}
		return conjugate() / nSq;
	}

	template <typename T>
	T Quaternion<T>::norm() const
	{
		return std::sqrt((r * r) + ijk.normSq());
	}

	template <typename T>
	T Quaternion<T>::normSq() const
	{
		return (r * r) + ijk.normSq();
	}

	template <typename T>
	void Quaternion<T>::normalize()
	{
		T n = norm();
		if (n == 0)
		{
			throw std::exception();
		}
		r /= n;
		ijk /= n;
	}

	template <typename T>
	Vector<3, T> Quaternion<T>::rotate(Vector<3, T> const & v) const
	{
		Vector<3, T> t = (T)2 * ijk.cross(v);
		return v + r * t + ijk.cross(t);
	}

	template <typename T>
	Vector<3, T> Quaternion<T>::getAxis(unsigned int i) const
	{
		if (i >= 3)
		{
			throw std::exception();
		}
		Vector3f axis;
		unsigned int j = (i + 1) % 3;
		unsigned int k = (i + 2) % 3;
		axis[i] = 1.0f - 2.0f * (ijk[j] * ijk[j] + ijk[k] * ijk[k]);
		axis[j] = 2.0f * (ijk[i] * ijk[j] + ijk[k] * r);
		axis[k] = 2.0f * (ijk[i] * ijk[k] - ijk[j] * r);
		return axis;
	}

	template <typename T>
	Matrix<3, 3, T> Quaternion<T>::getMatrix() const
	{
		Matrix<3, 3, T> m;
		float ii = ijk[0] * ijk[0];
		float ij = ijk[0] * ijk[1];
		float ik = ijk[0] * ijk[2];
		float ir = ijk[0] * r;
		float jj = ijk[1] * ijk[1];
		float jk = ijk[1] * ijk[2];
		float jr = ijk[1] * r;
		float kk = ijk[2] * ijk[2];
		float kr = ijk[2] * r;
		m(0, 0) = (T)1 - (T)2 * (jj + kk);
		m(0, 1) = (T)2 * (ij - kr);
		m(0, 2) = (T)2 * (ik + jr);
		m(1, 0) = (T)2 * (ij + kr);
		m(1, 1) = (T)1 - (T)2 * (ii + kk);
		m(1, 2) = (T)2 * (jk - ir);
		m(2, 0) = (T)2 * (ik - jr);
		m(2, 1) = (T)2 * (jk + ir);
		m(2, 2) = (T)1 - (T)2 * (ii + jj);
		return m;
	}

	template <typename T>
	Quaternion<T> operator + (Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs)
	{
		return Quaternion<T>(q_lhs.r + q_rhs.r, q_lhs.ijk + q_rhs.ijk);
	}

	template <typename T>
	Quaternion<T> operator - (Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs)
	{
		return Quaternion<T>(q_lhs.r - q_rhs.r, q_lhs.ijk - q_rhs.ijk);
	}

	template <typename T>
	Quaternion<T> operator * (Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs)
	{
		return Quaternion<T>(q_lhs.r * q_rhs.r - q_lhs.ijk.dot(q_rhs.ijk), q_lhs.r * q_rhs.ijk + q_rhs.r * q_lhs.ijk + q_lhs.ijk.cross(q_rhs.ijk));
	}

	template <typename T>
	Quaternion<T> operator * (Quaternion<T> const & q, T t)
	{
		return Quaternion<T>(q.r * t, q.ijk * t);
	}

	template <typename T>
	Quaternion<T> operator * (T t, Quaternion<T> const & q)
	{
		return Quaternion<T>(t * q.r, t * q.ijk);
	}
}

