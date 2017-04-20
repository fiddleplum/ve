#include "render/mesh.hpp"
#include "render/open_gl.hpp"

namespace ve
{
	namespace render
	{
		Mesh::Mesh()
		{
			numIndicesPerPrimitive = 3;
			numIndices = 0;
			glMode = GL_TRIANGLES;
			glGenVertexArrays(1, &vertexArrayObject);
			glGenBuffers(1, &indexBufferObject);
		}

		Mesh::~Mesh()
		{
			for (auto && pair : vertexBufferObjects)
			{
				glDeleteBuffers(1, &pair.second);
			}
			glDeleteVertexArrays(1, &vertexArrayObject);
			glDeleteBuffers(1, &indexBufferObject);
		}

		unsigned int Mesh::getNumIndicesPerPrimitive() const
		{
			return numIndicesPerPrimitive;
		}

		void Mesh::setNumIndicesPerPrimitive(unsigned int numIndices)
		{
			numIndicesPerPrimitive = numIndices;
			switch (numIndices)
			{
				case 1:
					glMode = GL_POINTS; break;
				case 2:
					glMode = GL_LINES; break;
				case 3:
					glMode = GL_TRIANGLES; break;
				default:
					throw std::runtime_error("Error: Wrong number of indices per primitive. ");
			}
		}

		void Mesh::setVertexComponent(unsigned int componentIndex, unsigned int numDimensions, unsigned int byteOffsetInVertex, unsigned int verticesIndex)
		{
			glBindVertexArray(vertexArrayObject);
			glVertexAttribFormat(componentIndex, numDimensions, GL_FLOAT, GL_FALSE, byteOffsetInVertex);
			glVertexAttribBinding(componentIndex, verticesIndex);
			glEnableVertexAttribArray(componentIndex);
		}

		void Mesh::setVertices(unsigned int index, std::vector<float> const & vertices, unsigned int byteSizeOfVertex)
		{
			auto & it = vertexBufferObjects.find(index);
			if (vertices.size() > 0 && byteSizeOfVertex > 0)
			{
				unsigned int vertexBufferObject;
				if (it == vertexBufferObjects.end())
				{
					glBindVertexArray(vertexArrayObject);
					glGenBuffers(1, &vertexBufferObject);
					glBindVertexBuffer(index, vertexBufferObject, 0, byteSizeOfVertex);
					vertexBufferObjects.insert(std::pair<unsigned int, unsigned int>(index, vertexBufferObject));
				}
				else
				{
					vertexBufferObject = it->second;
				}
				glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void const *)&vertices[0], GL_STATIC_DRAW);
			}
			else if (it != vertexBufferObjects.end())
			{
				unsigned int vertexBufferObject = it->second;
				vertexBufferObjects.erase(it);
				glBindVertexArray(vertexArrayObject);
				glDisableVertexAttribArray(index);
				glBindVertexBuffer(index, 0, 0, 0);
				glDeleteBuffers(1, &vertexBufferObject);
			}
		}

		void Mesh::setIndices(std::vector<unsigned int> const & indices)
		{
			numIndices = (unsigned int)indices.size();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), (void const *)&indices[0], GL_STATIC_DRAW);
		}

		void Mesh::render() const
		{
			glBindVertexArray(vertexArrayObject);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
			glDrawElements(glMode, numIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}