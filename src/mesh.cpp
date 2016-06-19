#include "mesh.h"
#include "serialize.h"
#include <fstream>

namespace ve
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(std::string const & filename)
	{
		std::fstream in(filename, std::fstream::in | std::fstream::binary);
		deserialize(in, numIndicesPerPrimitive);
		deserialize(in, numVerticesPerFrame);
		deserialize(in, formatTypes);
		deserialize(in, vertices);
		deserialize(in, indices);
	}

	void Mesh::save(std::string const & filename) const
	{
		std::fstream out(filename, std::fstream::out | std::fstream::binary);
		serialize(out, numIndicesPerPrimitive);
		serialize(out, numVerticesPerFrame);
		serialize(out, formatTypes);
		serialize(out, vertices);
		serialize(out, indices);
	}
}

