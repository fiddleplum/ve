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

		// Saves the mesh to a file.
		virtual void save(std::string const & filename) const = 0;

		// Returns the number of consecutive indices that make a single primitive. 1 means points, 2 means lines, 3 means triangles.
		virtual int getNumIndicesPerPrimitive() const = 0;

		// Sets the number of consecutive indices that make a single primitive. 1 means points, 2 means lines, 3 means triangles.
		virtual void setNumIndicesPerPrimitive(int numIndicesPerPrimitive) = 0;

		// Returns the number of vertices in a single animation frame. Zero means the whole set of vertices is used.
		virtual int getNumVerticesPerFrame() const = 0;

		// Sets the number of vertices in a single animation frame. Zero means the whole set of vertices is used.
		virtual void setNumVerticesPerFrame(int numVerticesPerFrame) = 0;

		// Returns the format that defines the types and ordering of the components of the vertices.
		virtual std::vector<int> const & getFormatTypes() const = 0;

		// Sets the format that defines the types and ordering of the components of the vertices.
		virtual void setFormatTypes(std::vector<int> const & formatTypes) = 0;

		// Returns the vertices of the mesh. The format types determine how the floats make up a vertex.
		virtual std::vector<float> const & getVertices() const = 0;

		// Sets the vertices of the mesh. The format types determine how the floats make up a vertex.
		virtual void setVertices(std::vector<float> const & vertices) = 0;

		// Returns the indices which each refer to an index in the vertex array.
		virtual std::vector<unsigned int> const & getIndices() const = 0;

		// Sets the indices which each refer to an index in the vertex array.
		virtual void setIndices(std::vector<unsigned int> const & indices) = 0;

	protected:
		virtual ~Mesh() = default;
	};
}