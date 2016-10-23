#pragma once

#include <string>
#include <vector>

namespace ve
{
	// A mesh contains the vertices and indices that form a set of primitives. Each vertex contains one or more components.
	class Mesh
	{
	public:
		// All of the possible types for a given vertex component. For shader and vbo attributes, the GL locations are the ordinals of the enums.
		enum FormatType { POSITION_3D, POSITION_2D, NORMAL, TANGENT, COLOR0_RGB, COLOR0_RGBA, COLOR1_RGB, COLOR1_RGBA, UV0, UV1, UV2, UV3 };

		// Constructs an empty mesh.
		Mesh();

		// Constructs from a .mesh file. The file format is described below this class.
		Mesh(std::string const & filename);

		void save(std::string const & filename) const;

		// The number of consecutive indices that make a single polygon. 1 means points, 2 means lines, 3 means triangles.
		int numIndicesPerPrimitive = 3;

		// The number of vertices in a single animation frame. Zero means the whole set of vertices is used.
		int numVerticesPerFrame = 0;

		// The format defines the types and ordering of the components of the vertices.
		std::vector<unsigned int> formatTypes;

		// The vertices contain the actual vertex data, typically one vertex per unique position in the mesh.
		std::vector<float> vertices;

		// The indices each refer to an index in the vertex array.
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

