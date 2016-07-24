#include "material.h"
#include "vertex_buffer_object.h"

namespace ve
{
	class Model
	{
	public:
		Model();

		virtual ~Model();

		UsePtr<Material> getMaterial() const;

		void setMaterial(UsePtr<Material> material);

		UsePtr<VertexBufferObject> getVertexBufferObject() const;

		void setVertexBufferObject(UsePtr<VertexBufferObject> vertexBufferObject);

		void render() const;

	private:
		UsePtr<Material> material;
		UsePtr<VertexBufferObject> vertexBufferObject;
	};
}

