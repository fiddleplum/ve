#pragma once

#include "render/mesh.h"

namespace ve
{
	class MeshInternal : public virtual Mesh
	{
	public:
		MeshInternal();

		MeshInternal(std::string const & filename);

		void save(std::string const & filename) const override;

		unsigned int getNumIndicesPerPrimitive() const override;

		void setNumIndicesPerPrimitive(unsigned int numIndicesPerPrimitive) override;

		unsigned int getNumVerticesPerFrame() const override;

		void setNumVerticesPerFrame(unsigned int numVerticesPerFrame) override;

		std::vector<unsigned int> const & getFormatTypes() const override;

		void setFormatTypes(std::vector<unsigned int> const & formatTypes) override;

		std::vector<float> const & getVertices() const override;

		void setVertices(std::vector<float> const & vertices) override;

		std::vector<unsigned int> const & getIndices() const override;

		void setIndices(std::vector<unsigned int> const & indices) override;

	private:
		unsigned int numIndicesPerPrimitive;
		unsigned int numVerticesPerFrame;
		std::vector<unsigned int> formatTypes;
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
	};
}

/*
* Notes on the Mesh file format. All ints and floats are 32-bit.
*
* unsigned int numIndicesPerPrimitive
* unsigned int numVerticesPerFrame
* unsigned int formatTypes.length
* unsigned int[] formatTypes
* unsigned int vertices.length (number of floats)
* float[] vertices
* unsigned int indices.length
* unsigned int[] indices
*/