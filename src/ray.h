#pragma once

#include "vector.h"

namespace ve
{
	template <unsigned int dim, typename T>
	class Ray
	{
	public:
		Ray();
		Ray(Vector<dim, T> start, Vector<dim, T> offset);

		Ray<dim, T> scaled(float factor);  // only affects direction

		Vector<dim, T> start;
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

