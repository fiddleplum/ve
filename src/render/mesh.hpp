#pragma once

#include <vector>
#include <map>

namespace ve
{
	namespace render
	{
		// A mesh contains the vertices and indices that form a set of primitives. Each vertex contains one or more components.
		class Mesh final
		{
		public:
			// Constructs an empty mesh.
			Mesh();

			// Destructor.
			~Mesh();

			// Gets the number of consecutive indices that make a single primitive. 1 for points, 2 for lines, and 3 for triangles.
			unsigned int getNumIndicesPerPrimitive() const;

			// Sets the number of consecutive indices that make a single primitive. 1 for points, 2 for lines, and 3 for triangles.
			void setNumIndicesPerPrimitive(unsigned int numIndices);

			// Sets the vertex component of the given index. The component index is used for the glsl layout attribute specifier. If instanced, then each value for this component in the vertices will be per instance rather than per vertex.
			void setVertexComponent(unsigned int componentIndex, unsigned int numDimensions, unsigned int byteOffsetInVertex, unsigned int verticesIndex, bool instanced);

			// Sets the vertices at a given index.
			void setVertices(unsigned int index, std::vector<float> const & vertices, unsigned int byteSizeOfVertex);

			// Gets the indices.
			std::vector<unsigned int> const & getIndices() const;

			// Sets the indices of a single instance.
			void setIndices(std::vector<unsigned int> const & indices);

			// Sets the number of instances to render.
			void setNumInstances(unsigned int numInstances);

			// Renders the Mesh.
			void render() const;

		private:
			unsigned int numIndicesPerPrimitive;
			unsigned int numIndicesInInstance;
			unsigned int numInstances;
			unsigned int glMode;
			unsigned int vertexArrayObject;
			std::map<unsigned int, unsigned int> vertexBufferObjects;
			unsigned int indexBufferObject;
		};
	}
}

