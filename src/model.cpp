#include "model.h"

namespace ve
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	UsePtr<Material> Model::getMaterial() const
	{
		return material;
	}

	void Model::setMaterial(UsePtr<Material> material_)
	{
		material = material_;
	}

	UsePtr<VertexBufferObject> Model::getVertexBufferObject() const
	{
		return vertexBufferObject;
	}

	void Model::setVertexBufferObject(UsePtr<VertexBufferObject> vertexBufferObject_)
	{
		vertexBufferObject = vertexBufferObject_;
	}

	void Model::render() const
	{
		if (material)
		{
			material->activate();
		}
		if (vertexBufferObject)
		{
			vertexBufferObject->render();
		}
	}
}

