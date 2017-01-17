#include "render/vertex_buffer_object.hpp"
#include "render/open_gl.hpp"
#include <map>

namespace ve
{
	namespace render
	{
		struct VertexArrayObject
		{
			unsigned int count;
			unsigned int vertexArrayObject;
		};

		std::map<std::string, VertexArrayObject> vertexArrayObjects;

		VertexBufferObject::VertexBufferObject(Mesh const & mesh)
		{
			switch (mesh.numIndicesPerPrimitive)
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
			std::vector<VertexComponent> vertexComponents;
			for (unsigned int i = 0; i < mesh.formatTypes.size(); i++)
			{
				auto type = mesh.formatTypes[i];
				vertexFormat += (type < 10 ? (char)(type + '0') : (char)(type - 10 + 'A'));
				unsigned int sizeOfComponent = 0;
				switch (type)
				{
					case Mesh::POSITION_2D:
					case Mesh::UV0:
					case Mesh::UV1:
					case Mesh::UV2:
					case Mesh::UV3:
						sizeOfComponent = 2; break;
					case Mesh::POSITION_3D:
					case Mesh::NORMAL:
					case Mesh::TANGENT:
					case Mesh::COLOR0_RGB:
					case Mesh::COLOR1_RGB:
						sizeOfComponent = 3; break;
					case Mesh::COLOR0_RGBA:
					case Mesh::COLOR1_RGBA:
						sizeOfComponent = 4; break;
				}
				vertexComponents.push_back({type, sizeOfComponent, bytesPerVertex});
				bytesPerVertex += sizeOfComponent * sizeof(float);
			}

			auto vertexArrayObjectIter = vertexArrayObjects.find(vertexFormat);
			if (vertexArrayObjectIter == vertexArrayObjects.end())
			{
				glGenVertexArrays(1, &vertexArrayObject);
				glBindVertexArray(vertexArrayObject);
				for (auto & vertexComponent : vertexComponents)
				{
					glEnableVertexAttribArray(vertexComponent.index);
					glVertexAttribFormat(vertexComponent.index, vertexComponent.size, GL_FLOAT, GL_FALSE, vertexComponent.offset);
					glVertexAttribBinding(vertexComponent.index, 0);
				}
				vertexArrayObjects[vertexFormat] = {1, vertexArrayObject};
			}
			else
			{
				vertexArrayObject = vertexArrayObjectIter->second.vertexArrayObject;
				vertexArrayObjectIter->second.count++;
			}
			glGenBuffers(1, &vertexBuffer);
			glBindVertexBuffer(0, vertexBuffer, 0, bytesPerVertex);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			updateVertices(mesh.vertices);
			numIndices = (int)mesh.indices.size();
			glGenBuffers(1, &indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)mesh.indices.size() * sizeof(unsigned int), (void const *)&mesh.indices[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
		}

		VertexBufferObject::~VertexBufferObject()
		{
			vertexArrayObjects[vertexFormat].count--;
			if (vertexArrayObjects[vertexFormat].count == 0)
			{
				glDeleteVertexArrays(1, &vertexArrayObject);
				vertexArrayObjects.erase(vertexFormat);
			}
			glDeleteBuffers(1, &indexBuffer);
			glDeleteBuffers(1, &vertexBuffer);
		}

		void VertexBufferObject::updateVertices(std::vector<float> const & vertices)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void const *)&vertices[0], GL_STATIC_DRAW);
		}

		void VertexBufferObject::render() const
		{
			glBindVertexArray(vertexArrayObject);
			glBindVertexBuffer(0, vertexBuffer, 0, bytesPerVertex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glDrawElements(mode, numIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}