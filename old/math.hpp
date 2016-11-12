#pragma once

#include <array>

namespace math
{
	// All angles are in radians.
	// 2-dimensional angles are counter clock-wise positive.

	using Vector2i = std::array<int, 2>;
	using Vector2f = std::array<float, 2>;
	using Vector2d = std::array<double, 2>;
	using Vector3i = std::array<int, 3>;
	using Vector3f = std::array<float, 3>;
	using Vector3d = std::array<double, 3>;
	using Vector4i = std::array<int, 4>;
	using Vector4f = std::array<float, 4>;
	using Vector4d = std::array<double, 4>;
	using Matrix33f = std::array<std::array<float, 3>, 3>;
	using Matrix33d = std::array<std::array<double, 3>, 3>;
	using Matrix44f = std::array<std::array<float, 4>, 4>;
	using Matrix44d = std::array<std::array<double, 4>, 4>;

	// Returns zero vector.
	template <class T, std::size_t N>
	std::array<T, N> zero();

	// Returns a unit axis.
	template <class T, std::size_t N>
	std::array<T, N> axis(std::size_t axis);

	// Returns vector filled with a.
	template <class T, std::size_t N>
	std::array<T, N> filled(T a);

	// Returns vector of type T as type R.
	template <class T, class R, std::size_t N>
	std::array<R, N> convert(std::array<T, N> const & c);

	// Normalizes c.
	template <class T, std::size_t N>
	void normalize(std::array<T, N> & c);

	// Returns the normalized vector of c.
	template <class T, std::size_t N>
	std::array<T, N> unit(std::array<T, N> const & c);

	// Returns v0 multiplied component-wise by v1.
	template <class T, std::size_t N>
	std::array<T, N> scale(std::array<T, N> const & v0, std::array<T, N> const & v1);

	// Returns v0 divided component-wise by v1.
	template <class T, std::size_t N>
	std::array<T, N> scaleInv(std::array<T, N> const & v0, std::array<T, N> const & v1);

	// Returns the dot product of v0 and v1.
	template <class T, std::size_t N>
	T dot(std::array<T, N> const & v0, std::array<T, N> const & v1);

	// Returns the norm (magnitude or length) of c.
	template <class T, std::size_t N>
	T norm(std::array<T, N> const & c);

	// Returns the 2D cross product of v0 and v1 (norm(v0) * norm(v1) * sin(angle from v0 to v1)).
	template <class T>
	T cross(std::array<T, 2> const & v0, std::array<T, 2> const & v1);

	// Returns the 3D cross product of v0 and v1.
	template <class T>
	std::array<T, 3> cross(std::array<T, 3> const & v0, std::array<T, 3> const & v1);

	// Returns vector that is v0 from the reference frame of v1 (as an x-axis) with a norm that is the product of the norms of v0 and v1.
	template <class T>
	std::array<T, 2> relative(std::array<T, 2> const & v0, std::array<T, 2> const & v1);

	// Returns vector rotated by the angle.
	template <class T>
	std::array<T, 2> rotate(std::array<T, 2> const & c, T angle);

	// Returns vector clamped component-wise to the range [min, max].
	template <class T, std::size_t N>
	std::array<T, N> clamp(std::array<T, N> c, T min, T max);

	// Returns vector clamped component-wise to the ranges in min and max.
	template <class T, std::size_t N>
	std::array<T, N> clamp(std::array<T, N> c, std::array<T, N> min, std::array<T, N> max);

	// Returns a zero matrix.
	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, ROWS>, COLS> zero();

	// Returns an identity matrix.
	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, ROWS>, COLS> identity();

	// Returns a matrix filled with a.
	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, ROWS>, COLS> filled(T a);

	// Returns the transpose of the matrix m.
	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, COLS>, ROWS> transpose(std::array<std::array<T, ROWS>, COLS> const & m);

	// Returns m * c without the last component, extending c either as a point (vLast = 1) or direction (vLast = 0).
	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<T, ROWS - 1> transform(std::array<std::array<T, ROWS>, COLS> const & m, std::array<T, COLS - 1> const & c, T vLast);

	// Returns the matrix m0 multiplied by the vector v1.
	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<T, ROWS> transform(std::array<std::array<T, ROWS>, COLS> const & m0, std::array<T, COLS> const & v1);

	// Returns the matrix m0 multiplied by the matrix m1.
	template <class T, std::size_t ROWS, std::size_t COLS, std::size_t M1_COLS>
	std::array<std::array<T, ROWS>, M1_COLS> transform(std::array<std::array<T, ROWS>, COLS> const & m0, std::array<std::array<T, COLS>, M1_COLS> const & m1);
}

// Returns true if v0 is component-wise equal to v1.
template <class T, std::size_t N>
bool operator == (std::array<T, N> const & v0, std::array<T, N> const & v1);

// Returns true if v0 is not component-wise equal to v1.
template <class T, std::size_t N>
bool operator != (std::array<T, N> const & v0, std::array<T, N> const & v1);

// Adds v0 component-wise by v1.
template <class T, std::size_t N>
void operator += (std::array<T, N> & v0, std::array<T, N> const & v1);

// Returns the component-wise sum of v0 and v1.
template <class T, std::size_t N>
std::array<T, N> operator + (std::array<T, N> const & v0, std::array<T, N> const & v1);

// Subtracts v0 component-wise by v1.
template <class T, std::size_t N>
void operator -= (std::array<T, N> & v0, std::array<T, N> const & v1);

// Returns the component-wise difference of v0 and v1.
template <class T, std::size_t N>
std::array<T, N> operator - (std::array<T, N> const & v0, std::array<T, N> const & v1);

// Multiplies v0 component-wise by v1.
template <class T, std::size_t N>
void operator *= (std::array<T, N> & v0, T v1);

// Returns the component-wise product of v0 and v1.
template <class T, std::size_t N>
std::array<T, N> operator * (std::array<T, N> const & v0, T v1);

// Multiplies v0 component-wise by v1.
template <class T, std::size_t N>
void operator /= (std::array<T, N> & v0, T v1);

// Returns the component-wise division of v0 and v1.
template <class T, std::size_t N>
std::array<T, N> operator / (std::array<T, N> const & v0, T v1);

// Returns the component-wise product of v0 and v1.
template <class T, std::size_t N>
std::array<T, N> operator * (T v0, std::array<T, N> const & v1);

// Template Implementations

namespace math
{
	template <class T, std::size_t N>
	std::array<T, N> zeroVector()
	{
		return filled<T, N>(0);
	}

	template <class T, std::size_t N>
	std::array<T, N> axis(std::size_t axis)
	{
		auto r = zero<T, N>();
		r.at(axis) = 1;
		return r;
	}

	template <class T, std::size_t N>
	std::array<T, N> filled(T a)
	{
		std::array<T, N> r;
		for (std::size_t i = 0; i < N; i++)
		{
			r[i] = a;
		}
		return r;
	}

	template <class T, class R, std::size_t N>
	std::array<R, N> convert(std::array<T, N> const & c)
	{
		std::array<R, N> r;
		for (std::size_t i = 0; i < N; i++)
		{
			r[i] = (R)c[i];
		}
		return r;
	}

	template <class T, std::size_t N>
	void normalize(std::array<T, N> & c)
	{
		T n = norm(c);
		c /= n;
	}

	template <class T, std::size_t N>
	std::array<T, N> unit(std::array<T, N> const & c)
	{
		T n = norm(c);
		return c / n;
	}

	template <class T, std::size_t N>
	std::array<T, N> scale(std::array<T, N> const & v0, std::array<T, N> const & v1)
	{
		std::array<T, N> r {v0};
		for (std::size_t i = 0; i < N; i++)
		{
			r[i] *= v1[i];
		}
		return r;
	}

	template <class T, std::size_t N>
	std::array<T, N> scaleInv(std::array<T, N> const & v0, std::array<T, N> const & v1)
	{
		std::array<T, N> r {v0};
		for (std::size_t i = 0; i < N; i++)
		{
			r[i] /= v1[i];
		}
		return r;
	}

	template <class T, std::size_t N>
	T dot(std::array<T, N> const & v0, std::array<T, N> const & v1)
	{
		T r {0};
		for (std::size_t i = 0; i < N; i++)
		{
			r += v0[i] * v1[i];
		}
		return r;
	}

	template <class T, std::size_t N>
	T norm(std::array<T, N> const & c)
	{
		return std::sqrt(dot(c, c));
	}

	template <class T>
	T cross(std::array<T, 2> const & v0, std::array<T, 2> const & v1)
	{
		return v0[0] * v1[1] - v0[1] * v1[0];
	}

	template <class T>
	std::array<T, 3> cross(std::array<T, 3> const & v0, std::array<T, 3> const & v1)
	{
		std::array<T, 3> r;
		r[0] = v0[1] * v1[2] - v0[2] * v1[1];
		r[1] = v0[2] * v1[0] - v0[0] * v1[2];
		r[2] = v0[0] * v1[1] - v0[1] * v1[0];
		return r;
	}

	template <class T>
	std::array<T, 2> relative(std::array<T, 2> const & v0, std::array<T, 2> const & v1)
	{
		return {dot(v0, v1), cross(v0, v1)};
	}

	template <class T>
	std::array<T, 2> rotate(std::array<T, 2> const & c, T angle)
	{
		T cosa = std::cos(angle);
		T sina = std::sin(angle);
		return {c[0] * cosa - c[1] * sina, c[0] * sina + c[1] * cosa};
	}

	template <class T, std::size_t N>
	std::array<T, N> clamp(std::array<T, N> c, T min, T max)
	{
		std::array<T, N> r;
		for (std::size_t i = 0; i < N; i++)
		{
			if (c[i] < min)
			{
				r[i] = min;
			}
			else if (c[i] > max)
			{
				r[i] = max;
			}
			else
			{
				r[i] = c[i];
			}
		}
		return r;
	}

	template <class T, std::size_t N>
	std::array<T, N> clamp(std::array<T, N> c, std::array<T, N> min, std::array<T, N> max)
	{
		std::array<T, N> r;
		for (std::size_t i = 0; i < N; i++)
		{
			if (c[i] < min[i])
			{
				r[i] = min[i];
			}
			else if (c[i] > max[i])
			{
				r[i] = max[i];
			}
			else
			{
				r[i] = c[i];
			}
		}
		return r;
	}

	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, ROWS>, COLS> zero()
	{
		return filled<T, ROWS, COLS>(0);
	}

	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, ROWS>, COLS> identity()
	{
		auto r = zero<T, ROWS, COLS>();
		for (std::size_t col = 0; col < COLS && col < ROWS; col++)
		{
			r[col][col] = 1;
		}
		return r;
	}

	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, ROWS>, COLS> filled(T a)
	{
		std::array<std::array<T, ROWS>, COLS> r;
		for (std::size_t col = 0; col < COLS; col++)
		{
			for (std::size_t row = 0; row < ROWS; row++)
			{
				r[col][row] = a;
			}
		}
		return r;
	}

	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<std::array<T, COLS>, ROWS> transpose(std::array<std::array<T, ROWS>, COLS> const & m)
	{
		std::array<std::array<T, COLS>, ROWS> r;
		for (std::size_t col = 0; col < COLS; col++)
		{
			for (std::size_t row = 0; row < ROWS; row++)
			{
				r[row][col] = m[col][row];
			}
		}
		return r;
	}

	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<T, ROWS - 1> transform(std::array<std::array<T, ROWS>, COLS> const & m, std::array<T, COLS - 1> const & c, T vLast)
	{
		std::array<T, ROWS - 1> r = zero<T, ROWS - 1>();
		for (std::size_t row = 0; row < ROWS - 1; row++)
		{
			for (std::size_t col = 0; col < COLS - 1; col++)
			{
				r[row] += m[col][row] * c[col];
			}
			r[row] += m[COLS - 1][row] * vLast;
		}
		return r;
	}

	template <class T, std::size_t ROWS, std::size_t COLS>
	std::array<T, ROWS> transform(std::array<std::array<T, ROWS>, COLS> const & m0, std::array<T, COLS> const & v1)
	{
		std::array<T, ROWS> r = math::zero<T, ROWS>();
		for (std::size_t row = 0; row < ROWS; row++)
		{
			for (std::size_t col = 0; col < COLS; col++)
			{
				r[row] += m0[col][row] * v1[col];
			}
		}
		return r;
	}

	template <class T, std::size_t ROWS, std::size_t COLS, std::size_t M1_COLS>
	std::array<std::array<T, ROWS>, M1_COLS> transform(std::array<std::array<T, ROWS>, COLS> const & m0, std::array<std::array<T, COLS>, M1_COLS> const & m1)
	{
		std::array<std::array<T, ROWS>, M1_COLS> r = math::zero<T, ROWS, M1_COLS>();
		for (std::size_t m1Cols = 0; m1Cols < M1_COLS; m1Cols)
		{
			for (std::size_t row = 0; row < ROWS; row++)
			{
				for (std::size_t col = 0; col < COLS; col++)
				{
					r[m1Cols][row] += m0[col][row] * m1[m1Cols][col];
				}
			}
		}
		return r;
	}
}

template <class T, std::size_t N>
bool operator == (std::array<T, N> const & v0, std::array<T, N> const & v1)
{
	for (std::size_t i = 0; i < N; i++)
	{
		if (v0[i] != v1[i])
		{
			return false;
		}
	}
	return true;
}

template <class T, std::size_t N>
bool operator != (std::array<T, N> const & v0, std::array<T, N> const & v1)
{
	return !(v0 == v1);
}

template <class T, std::size_t N>
void operator += (std::array<T, N> & v0, std::array<T, N> const & v1)
{
	for (std::size_t i = 0; i < N; i++)
	{
		v0[i] += v1[i];
	}
}

template <class T, std::size_t N>
std::array<T, N> operator + (std::array<T, N> const & v0, std::array<T, N> const & v1)
{
	std::array<T, N> r {v0};
	r += v1;
	return r;
}

template <class T, std::size_t N>
void operator -= (std::array<T, N> & v0, std::array<T, N> const & v1)
{
	for (std::size_t i = 0; i < N; i++)
	{
		v0[i] -= v1[i];
	}
}

template <class T, std::size_t N>
std::array<T, N> operator - (std::array<T, N> const & v0, std::array<T, N> const & v1)
{
	std::array<T, N> r {v0};
	r -= v1;
	return r;
}

template <class T, std::size_t N>
void operator *= (std::array<T, N> & v0, T v1)
{
	for (std::size_t i = 0; i < N; i++)
	{
		v0[i] *= v1;
	}
}

template <class T, std::size_t N>
std::array<T, N> operator * (std::array<T, N> const & v0, T v1)
{
	std::array<T, N> r {v0};
	r *= v1;
	return r;
}

template <class T, std::size_t N>
void operator /= (std::array<T, N> & v0, T v1)
{
	for (std::size_t i = 0; i < N; i++)
	{
		v0[i] /= v1;
	}
}

template <class T, std::size_t N>
std::array<T, N> operator / (std::array<T, N> const & v0, T v1)
{
	std::array<T, N> r {v0};
	r /= v1;
	return r;
}

template <class T, std::size_t N>
std::array<T, N> operator * (T v0, std::array<T, N> const & v1)
{
	std::array<T, N> r;
	for (std::size_t i = 0; i < N; i++)
	{
		r[i] = v0 * v1[i];
	}
	return r;
}