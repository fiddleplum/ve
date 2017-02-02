#pragma once

#include <string>
#include <vector>
#include <istream>

namespace ve
{
	namespace render
	{
		// A mesh contains the vertices and indices that form a set of primitives. Each vertex contains one or more components.
		class Mesh final
		{
		public:
			// The possible format types for a vertex component. For shader and vbo attributes, the GL locations are the ordinals of the enums.
			enum { POSITION_3D, POSITION_2D, NORMAL, TANGENT, COLOR0_RGB, COLOR0_RGBA, COLOR1_RGB, COLOR1_RGBA, UV0, UV1, UV2, UV3 };

			// Constructs an empty mesh.
			Mesh();

			// Constructs a mesh from a stream. The stream should be binary.
			Mesh(std::istream & in);

			// Destructor.
			~Mesh();

			// Saves the mesh to a stream. The stream should be binary.
			void save(std::ostream & out) const;

			// Gets the number of consecutive indices that make a single primitive. 1 for points, 2 for lines, and 3 for triangles.
			unsigned int getNumIndicesPerPrimitive() const;

			// Sets the number of consecutive indices that make a single primitive. 1 for points, 2 for lines, and 3 for triangles.
			void setNumIndicesPerPrimitive(unsigned int numIndices);

			// Gets the format that defines the types and ordering of the components of the vertices.
			std::vector<unsigned int> const & getVertexFormat() const;

			// Sets the format that defines the types and ordering of the components of the vertices.
			void setVertexFormat(std::vector<unsigned int> const & formatTypes);

			// Gets the vertices;
			std::vector<float> const & getVertices() const;

			// Sets the vertices.
			void setVertices(std::vector<float> const & vertices);

			// Gets the indices;
			std::vector<unsigned int> const & getIndices() const;

			// Sets the indices.
			void setIndices(std::vector<unsigned int> const & indices);

			// Renders the VBO.
			void render() const;

		private:
			unsigned int numIndicesPerPrimitive;
			unsigned int numVerticesPerFrame;
			std::vector<unsigned int> formatTypes;
			std::string vertexFormat;
			std::vector<float> vertices;
			std::vector<unsigned int> indices;

			unsigned int glMode;
			unsigned int vertexArrayObject;
			unsigned int vertexBuffer;
			unsigned int indexBuffer;
			unsigned int bytesPerVertex;
		};
	}
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