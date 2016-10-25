#include "mesh_internal.h"
#include "serialize.h"
#include <fstream>

namespace ve
{
	MeshInternal::MeshInternal()
	{
		numIndicesPerPrimitive = 3;
		numVerticesPerFrame = 0;
	}

	MeshInternal::MeshInternal(std::string const & filename)
	{
		std::fstream in(filename, std::fstream::in | std::fstream::binary);
		deserialize(in, numIndicesPerPrimitive);
		deserialize(in, numVerticesPerFrame);
		deserialize(in, formatTypes);
		deserialize(in, vertices);
		deserialize(in, indices);
	}

	void MeshInternal::save(std::string const & filename) const
	{
		std::fstream out(filename, std::fstream::out | std::fstream::binary);
		serialize(out, numIndicesPerPrimitive);
		serialize(out, numVerticesPerFrame);
		serialize(out, formatTypes);
		serialize(out, vertices);
		serialize(out, indices);
	}

	int MeshInternal::getNumIndicesPerPrimitive() const
	{
		return numIndicesPerPrimitive;
	}

	void MeshInternal::setNumIndicesPerPrimitive(int numIndicesPerPrimitive_)
	{
		numIndicesPerPrimitive = numIndicesPerPrimitive_;
	}

	int MeshInternal::getNumVerticesPerFrame() const
	{
		return numVerticesPerFrame;
	}

	void MeshInternal::setNumVerticesPerFrame(int numVerticesPerFrame_)
	{
		numVerticesPerFrame = numVerticesPerFrame_;
	}

	std::vector<int> const & MeshInternal::getFormatTypes() const
	{
		return formatTypes;
	}

	void MeshInternal::setFormatTypes(std::vector<int> const & formatTypes_)
	{
		formatTypes = formatTypes_;
	}

	std::vector<float> const & MeshInternal::getVertices() const
	{
		return vertices;
	}

	void MeshInternal::setVertices(std::vector<float> const & vertices_)
	{
		vertices = vertices_;
	}

	std::vector<unsigned int> const & MeshInternal::getIndices() const
	{
		return indices;
	}

	void MeshInternal::setIndices(std::vector<unsigned int> const & indices_)
	{
		indices = indices_;
	}
}