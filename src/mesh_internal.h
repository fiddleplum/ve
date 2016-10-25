#pragma once

#include "mesh.h"

namespace ve
{
	class MeshInternal : public virtual Mesh
	{
	public:
		MeshInternal();

		MeshInternal(std::string const & filename);

		void save(std::string const & filename) const override;

		int getNumIndicesPerPrimitive() const override;

		void setNumIndicesPerPrimitive(int numIndicesPerPrimitive) override;

		int getNumVerticesPerFrame() const override;

		void setNumVerticesPerFrame(int numVerticesPerFrame) override;

		std::vector<int> const & getFormatTypes() const override;

		void setFormatTypes(std::vector<int> const & formatTypes) override;

		std::vector<float> const & getVertices() const override;

		void setVertices(std::vector<float> const & vertices) override;

		std::vector<unsigned int> const & getIndices() const override;

		void setIndices(std::vector<unsigned int> const & indices) override;

	private:
		int numIndicesPerPrimitive;
		int numVerticesPerFrame;
		std::vector<int> formatTypes;
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
	};
}

/*
* Notes on the Mesh file format. All ints and floats are 32-bit.
*
* int numIndicesPerPrimitive
* int numVerticesPerFrame
* int formatTypes.length
* int[] formatTypes
* int vertices.length (number of floats)
* float[] vertices
* int indices.length
* int[] indices
*/