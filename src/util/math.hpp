#pragma once

namespace ve
{
	namespace math
	{
		double const PI = 3.1415926535897932384626433832795; //< 180 degrees
		double const TWO_PI = 6.283185307179586476925286766559; //< 360 degrees
		double const PI_OVER_2 = 1.5707963267948966192313216916398; //< 90 degrees
		double const PI_OVER_3 = 1.0471975511965977461542144610932; //< 60 degrees
		double const PI_OVER_4 = 0.78539816339744830961566084581988; //< 45 degrees
		double const PI_OVER_6 = 0.52359877559829887307710723054658; //< 30 degrees
		double const PI_OVER_12 = 0.26179938779914943653855361527329; //< 15 degrees

		//! Returns v clamped to between min and max.
		template <class T> T clamp(T v, T min, T max);

		//! Returns the maximum of x and y
		template <class T> T max(T x, T y);

		//! Returns the minimum of x and y
		template <class T> T min(T x, T y);

		//! Returns an int random number between min (inclusive) and max (exclusive).
		int random(int min, int max);

		//! Returns a float random number between min (inclusive) and max (inclusive). Has a resolution of (max - min) / RAND_MAX.
		float random(float min, float max);

		//! Returns the next highest power of two, given a 16 bit number.
		template <class T> T ceilPow2(T x);

		//! Returns the ceiling of the integer, given the period.
		int ceil(int i, int period);

		//! Returns true if v is not a number.
		template <class T> bool isNan(T v);

		//! Returns true if v is infinite.
		template <class T> bool isInf(T v);

		// Template Implementations.

		template <class T> T clamp(T v, T min, T max)
		{
			return v < min ? min : (v > max ? max : v);
		}

		template <class T> T max(T x, T y)
		{
			return x >= y ? x : y;
		}

		template <class T> T min(T x, T y)
		{
			return x <= y ? x : y;
		}

		template <class T> T ceilPow2(T x)
		{
			x--;
			x |= x >> 1;
			x |= x >> 2;
			x |= x >> 4;
			x |= x >> 8;
			x |= x >> 16;
			x++;
			return x;
		}

		template <class T> bool isNan(T v)
		{
#ifndef _MSC_VER
			return std::isnan(v);
#else
			return _isnan(v);
#endif
		}

		template <class T> bool isInf(T v)
		{
#ifndef _MSC_VER
			return std::isinf(v);
#else
			return _isinf(v);
#endif
		}
	}
}