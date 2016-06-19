#pragma once

#include "mesh.h"

namespace ve
{
	class VertexBufferObject
	{
	public:
		VertexBufferObject(UsePtr<Mesh> mesh);

		VertexBufferObject(std::string const & filename);

		~VertexBufferObject();

		void updateVertices(UsePtr<Mesh> mesh);

		void render() const;

		static Cache<VertexBufferObject> cache;

	private:
		void init(UsePtr<Mesh> mesh);

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

