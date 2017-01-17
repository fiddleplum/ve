#pragma once

#include "util/mesh.hpp"
#include "util/ptr.hpp"

namespace ve
{
	namespace render
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
				unsigned int offset;
			};
			std::string vertexFormat;
			unsigned int vertexArrayObject = 0;
			unsigned int vertexBuffer = 0;
			unsigned int indexBuffer = 0;
			unsigned int mode;
			unsigned int numIndices;
			unsigned int bytesPerVertex;
		};
	}
}