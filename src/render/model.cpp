#include "model.h"

namespace ve
{
	float Model::getDepth() const
	{
		return depth;
	}

	void Model::setDepth(float depth_)
	{
		depth = depth_;
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

	void Model::setUniformsFunction(std::function<void()> uniformsFunction_)
	{
		uniformsFunction = uniformsFunction_;
	}

	void Model::render() const
	{
		if (material)
		{
			material->activate();
		}
		if (uniformsFunction)
		{
			uniformsFunction();
		}
		if (vertexBufferObject)
		{
			vertexBufferObject->render();
		}
	}

	bool Model::operator < (Model const & model) const
	{
		return material < model.material;
	}

	bool operator < (UsePtr<Model> const & lhs, UsePtr<Model> const & rhs)
	{
		return *lhs < *rhs;
	}
}