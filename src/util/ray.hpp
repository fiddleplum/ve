#pragma once

#include "vector.hpp"

namespace ve
{
	// A ray is a starting point and a vector pointing in a direction.
	template <unsigned int dim, typename T>
	class Ray
	{
	public:
		// Constructs a ray with both start and offset zeroed.
		Ray();

		// Constructs a ray with start and offset specified.
		Ray(Vector<dim, T> start, Vector<dim, T> offset);

		// Returns the ray with its offset scaled by factor.
		Ray<dim, T> scaled(float factor);

		// The beginning of the ray.
		Vector<dim, T> start;

		// The end point or direction of the ray.
		Vector<dim, T> offset;
	};

	typedef Ray<2, int> Ray2i;
	typedef Ray<2, float> Ray2f;
	typedef Ray<2, double> Ray2d;
	typedef Ray<3, int> Ray3i;
	typedef Ray<3, float> Ray3f;
	typedef Ray<3, double> Ray3d;

	// Template implementation

	template <unsigned int dim, typename T>
	Ray<dim, T>::Ray()
	{
	}

	template <unsigned int dim, typename T>
	Ray<dim, T>::Ray(Vector<dim, T> newStart, Vector<dim, T> newOffset)
	{
		start = newStart;
		offset = newOffset;
	}

	template <unsigned int dim, typename T>
	Ray<dim, T> Ray<dim, T>::scaled(float factor)
	{
		Ray<dim, T> r;
		r.start = start;
		r.offset = offset * factor;
		return r;
	}
}