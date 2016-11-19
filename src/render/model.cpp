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

	Ptr<Material> Model::getMaterial() const
	{
		return material;
	}

	void Model::setMaterial(Ptr<Material> material_)
	{
		material = material_;
	}

	Ptr<VertexBufferObject> Model::getVertexBufferObject() const
	{
		return vertexBufferObject;
	}

	void Model::setVertexBufferObject(Ptr<VertexBufferObject> vertexBufferObject_)
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

	bool operator < (Ptr<Model> const & lhs, Ptr<Model> const & rhs)
	{
		return *lhs < *rhs;
	}
}