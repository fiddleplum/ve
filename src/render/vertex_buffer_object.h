#pragma once

#include "mesh.h"
#include "ptr.h"

namespace ve
{
	namespace render
	{
		class VertexBufferObject
		{
		public:
			// Constructs the VBO from a mesh.
			VertexBufferObject(UsePtr<Mesh> mesh);

			// Destructs the VBO.
			~VertexBufferObject();

			// Updates the vertices with a mesh's vertices. Does not update anything else from the mesh.
			void updateVertices(UsePtr<Mesh> mesh);

			// Renders the VBO.
			void render() const;

		private:
			struct VertexComponent
			{
			public:
				unsigned int index;
				unsigned int size;
				intptr_t offset;
			};
			unsigned int vertexBuffer;
			unsigned int indexBuffer;
			unsigned int mode;
			unsigned int numIndices;
			unsigned int bytesPerVertex;
			std::vector<VertexComponent> vertexComponents;
		};
	}
}

