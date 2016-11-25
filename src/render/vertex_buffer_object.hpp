#pragma once

#include "render/mesh.hpp"
#include "util/ptr.hpp"

namespace ve
{
	class VertexBufferObject
	{
	public:
		// Constructs the VBO from a mesh.
		VertexBufferObject(Mesh const & mesh);

		// Destructs the VBO.
		~VertexBufferObject();

		// Updates the vertices.
		void updateVertices(std::vector<float> const & vertices);

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