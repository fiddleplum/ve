#pragma once

#include "render/mesh.h"
#include "util/ptr.h"

namespace ve
{
	class VertexBufferObject
	{
	public:
		// Constructs the VBO from a mesh.
		VertexBufferObject(Ptr<Mesh> mesh);

		// Destructs the VBO.
		~VertexBufferObject();

		// Updates the vertices with a mesh's vertices. Does not update anything else from the mesh.
		void updateVertices(Ptr<Mesh> mesh);

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
		unsigned int vertexBuffer = 1;
		unsigned int indexBuffer = 2;
		unsigned int mode;
		unsigned int numIndices;
		unsigned int bytesPerVertex;
		std::vector<VertexComponent> vertexComponents;
	};
}