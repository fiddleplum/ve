#include "vertex_buffer_object.h"
#include "open_gl.h"

namespace ve
{
	VertexBufferObject::VertexBufferObject(UsePtr<Mesh> mesh)
	{
		init(mesh);
	}

	VertexBufferObject::VertexBufferObject(std::string const & filename)
	{
		UsePtr<Mesh> mesh = Mesh::cache.get(filename);
		if (!mesh.isValid())
		{
			mesh = Mesh::cache.create(filename, filename);
		}

		init(mesh);
	}

	VertexBufferObject::~VertexBufferObject()
	{
		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &vertexBuffer);
	}

	void VertexBufferObject::updateVertices(UsePtr<Mesh> mesh)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, 0, (void const *)&mesh->vertices[0], GL_STATIC_DRAW);
	}

	void VertexBufferObject::render() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		for (VertexComponent const & vertexComponent : vertexComponents)
		{
			glEnableVertexAttribArray(vertexComponent.index);
			glVertexAttribPointer(vertexComponent.index, vertexComponent.size, GL_FLOAT, GL_FALSE, bytesPerVertex, (void const *)vertexComponent.offset);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(mode, numIndices, GL_UNSIGNED_INT, 0);
	}

	void VertexBufferObject::init(UsePtr<Mesh> mesh)
	{
		switch (mesh->numIndicesPerPrimitive)
		{
		case 1:
			mode = GL_POINTS; break;
		case 2:
			mode = GL_LINES; break;
		case 3:
		default:
			mode = GL_TRIANGLES; break;
		}

		bytesPerVertex = 0;
		for (int i = 0; i < mesh->formatTypes.size(); i++)
		{
			unsigned int sizeOfComponent = 0;
			switch (mesh->formatTypes[i])
			{
			case Mesh::POSITION_XY:
			case Mesh::UV0:
			case Mesh::UV1:
			case Mesh::UV2:
			case Mesh::UV3:
				sizeOfComponent = 2; break;
			case Mesh::POSITION_XYZ:
			case Mesh::NORMAL:
			case Mesh::TANGENT:
			case Mesh::COLOR0_RGB:
			case Mesh::COLOR1_RGB:
				sizeOfComponent = 3; break;
			case Mesh::COLOR0_RGBA:
			case Mesh::COLOR1_RGBA:
				sizeOfComponent = 4; break;
			}
			vertexComponents.push_back({mesh->formatTypes[i], sizeOfComponent, bytesPerVertex});
			bytesPerVertex += sizeOfComponent;
		}

		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);
		updateVertices(mesh);
		numIndices = (unsigned int)mesh->indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), (void const *)&mesh->indices[0], GL_STATIC_DRAW);
	}
}
