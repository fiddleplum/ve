#include "render/mesh.hpp"
#include "util/serialize.hpp"
#include <istream>

namespace ve
{
	Mesh::Mesh(std::istream & in)
	{
		deserialize(in, numIndicesPerPrimitive);
		deserialize(in, numVerticesPerFrame);
		deserialize(in, formatTypes);
		deserialize(in, vertices);
		deserialize(in, indices);
	}

	void Mesh::save(std::ostream & out) const
	{
		serialize(out, numIndicesPerPrimitive);
		serialize(out, numVerticesPerFrame);
		serialize(out, formatTypes);
		serialize(out, vertices);
		serialize(out, indices);
	}
}