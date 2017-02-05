#include "render/mesh.hpp"
#include "render/open_gl.hpp"
#include "util/serialize.hpp"
#include <map>

namespace ve
{
	namespace render
	{
		struct VertexArrayObject
		{
			unsigned int count;
			unsigned int vertexArrayObject;
			unsigned int bytesPerVertex;
		};

		std::map<std::string, VertexArrayObject> vertexArrayObjects;

		Mesh::Mesh()
		{
			numIndicesPerPrimitive = 3;
			numVerticesPerFrame = 0;
			glMode = GL_TRIANGLES;
			vertexArrayObject = 0;
			glGenBuffers(1, &vertexBuffer);
			glGenBuffers(1, &indexBuffer);
			bytesPerVertex = 0;
		}

		Mesh::Mesh(std::istream & in)
			: Mesh()
		{
			unsigned int numIndicesPerPrimitive;
			unsigned int numVerticesPerFrame;
			std::vector<unsigned int> formatTypes;
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			deserialize(in, numIndicesPerPrimitive);
			deserialize(in, numVerticesPerFrame);
			deserialize(in, formatTypes);
			deserialize(in, vertices);
			deserialize(in, indices);
			setNumIndicesPerPrimitive(numIndicesPerPrimitive);
			setVertexFormat(formatTypes);
			setVertices(vertices);
			setIndices(indices);
		}

		Mesh::~Mesh()
		{
			if (vertexFormat.size() > 0)
			{
				vertexArrayObjects[vertexFormat].count--;
				if (vertexArrayObjects[vertexFormat].count == 0)
				{
					glDeleteVertexArrays(1, &vertexArrayObject);
					vertexArrayObjects.erase(vertexFormat);
				}
			}
			glDeleteBuffers(1, &indexBuffer);
			glDeleteBuffers(1, &vertexBuffer);
		}

		void Mesh::save(std::ostream & out) const
		{
			serialize(out, numIndicesPerPrimitive);
			serialize(out, numVerticesPerFrame);
			serialize(out, formatTypes);
			serialize(out, vertices);
			serialize(out, indices);
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

		std::vector<unsigned int> const & Mesh::getVertexFormat() const
		{
			return formatTypes;
		}

		void Mesh::setVertexFormat(std::vector<unsigned int> const & formatTypes_)
		{
			formatTypes = formatTypes_;
			vertexFormat.clear();
			for (unsigned int i = 0; i < formatTypes.size(); i++)
			{
				auto && type = formatTypes[i];
				vertexFormat += (type < 10 ? (char)(type + '0') : (char)(type - 10 + 'A'));
			}

			auto vertexArrayObjectIter = vertexArrayObjects.find(vertexFormat);
			if (vertexArrayObjectIter == vertexArrayObjects.end())
			{
				glGenVertexArrays(1, &vertexArrayObject);
				glBindVertexArray(vertexArrayObject);
				bytesPerVertex = 0;
				for (unsigned int i = 0; i < formatTypes.size(); i++)
				{
					auto && type = formatTypes[i];
					unsigned int sizeOfComponent = 0;
					switch (type)
					{
						case POSITION_2D:
						case UV0:
						case UV1:
						case UV2:
						case UV3:
							sizeOfComponent = 2; break;
						case POSITION_3D:
						case NORMAL:
						case TANGENT:
						case COLOR0_RGB:
						case COLOR1_RGB:
							sizeOfComponent = 3; break;
						case COLOR0_RGBA:
						case COLOR1_RGBA:
							sizeOfComponent = 4; break;
					}
					glEnableVertexAttribArray(type); // The type is also the index.
					glVertexAttribFormat(type, sizeOfComponent, GL_FLOAT, GL_FALSE, bytesPerVertex);
					glVertexAttribBinding(type, 0);
					bytesPerVertex += sizeOfComponent * sizeof(float);
				}
				vertexArrayObjects[vertexFormat] = {1, vertexArrayObject, bytesPerVertex};
			}
			else
			{
				vertexArrayObject = vertexArrayObjectIter->second.vertexArrayObject;
				bytesPerVertex = vertexArrayObjectIter->second.bytesPerVertex;
				vertexArrayObjectIter->second.count++;
			}
			glBindVertexBuffer(0, vertexBuffer, 0, bytesPerVertex);
		}

		std::vector<float> const & Mesh::getVertices() const
		{
			return vertices;
		}

		void Mesh::setVertices(std::vector<float> const & vertices_)
		{
			vertices = vertices_;
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void const *)&vertices[0], GL_STATIC_DRAW);
		}

		std::vector<unsigned int> const & Mesh::getIndices() const
		{
			return indices;
		}

		void Mesh::setIndices(std::vector<unsigned int> const & indices_)
		{
			indices = indices_;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), (void const *)&indices[0], GL_STATIC_DRAW);
		}

		void Mesh::render() const
		{
			glBindVertexArray(vertexArrayObject);
			glBindVertexBuffer(0, vertexBuffer, 0, bytesPerVertex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glDrawElements(glMode, (int)indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}