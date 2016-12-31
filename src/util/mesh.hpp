#pragma once

#include <string>
#include <vector>
#include <istream>

namespace ve
{
	// A mesh contains the vertices and indices that form a set of primitives. Each vertex contains one or more components.
	class Mesh final
	{
	public:
		// All of the possible types for a given vertex component. For shader and vbo attributes, the GL locations are the ordinals of the enums.
		enum FormatType { POSITION_3D, POSITION_2D, NORMAL, TANGENT, COLOR0_RGB, COLOR0_RGBA, COLOR1_RGB, COLOR1_RGBA, UV0, UV1, UV2, UV3 };

		// Constructs an empty mesh.
		Mesh() = default;

		// Constructs a mesh from a stream. The stream should be binary.
		Mesh(std::istream & in);

		// Saves the mesh to a stream. The stream should be binary.
		void save(std::ostream & out) const;

		// The number of consecutive indices that make a single primitive. 1 means points, 2 means lines, 3 means triangles.
		unsigned int numIndicesPerPrimitive = 3;

		// The number of vertices in a single animation frame. Zero means the whole set of vertices is used.
		unsigned int numVerticesPerFrame = 0;

		// The format that defines the types and ordering of the components of the vertices.
		std::vector<unsigned int> formatTypes;

		// The vertices of the mesh. The format types determine how the floats make up a vertex.
		std::vector<float> vertices;

		// The indices which each refer to an index in the vertex array.
		std::vector<unsigned int> indices;
	};
}

/*
* Notes on the Mesh binary format. All ints and floats are 32-bit.
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