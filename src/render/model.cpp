#include "model.hpp"

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

	Ptr<VertexBufferObject> Model::getVertexBufferObject() const
	{
		return vertexBufferObject;
	}

	void Model::setVertexBufferObject(Ptr<VertexBufferObject> const & vertexBufferObject_)
	{
		vertexBufferObject = vertexBufferObject_;
	}

	Ptr<Shader> Model::getShader() const
	{
		return shader;
	}

	void Model::setShader(Ptr<Shader> const & shader_)
	{
		shader = shader_;
	}
	
	Ptr<Texture> Model::getTextureAtSlot(unsigned int slot) const
	{
		if (slot >= textures.size())
		{
			return Ptr<Texture>();
		}
		else
		{
			return textures[slot];
		}
	}

	void Model::setTextureAtSlot(Ptr<Texture> const & texture, unsigned int slot)
	{
		if (slot >= textures.size())
		{
			textures.resize(slot + 1);
		}
		textures[slot] = texture;
	}

	void Model::setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction_)
	{
		uniformsFunction = uniformsFunction_;
	}

	void Model::render(std::function<void(Ptr<Shader> const &)> const & stageUniformsFunction, std::function<void(Ptr<Shader> const &)> const & sceneUniformsFunction) const
	{
		if (!shader || !vertexBufferObject)
		{
			return;
		}
		bool newShader = shader->activate();
		if (newShader)
		{
			if(stageUniformsFunction)
			{
				stageUniformsFunction(shader);
			}
			if (sceneUniformsFunction)
			{
				sceneUniformsFunction(shader);
			}
		}
		if (uniformsFunction)
		{
			uniformsFunction(shader);
		}
		for (unsigned int slot = 0; slot < textures.size(); slot++)
		{
			if (textures[slot].isValid())
			{
				textures[slot]->activate(slot);
			}
		}
		Texture::deactivateRest((unsigned int)textures.size());
		vertexBufferObject->render();
	}

	bool Model::operator < (Model const & model) const
	{
		if (!shader && model.shader)
		{
			return true;
		}
		if (!shader || !model.shader)
		{
			return false;
		}
		if (shader->getBlending() == Shader::Blending::NONE && model.shader->getBlending() != Shader::Blending::NONE)
		{
			return true;
		}
		if (shader->getBlending() != Shader::Blending::NONE && model.shader->getBlending() == Shader::Blending::NONE)
		{
			return false;
		}
		else if (shader->getBlending() == Shader::Blending::NONE)
		{
			if (shader != model.shader)
			{
				return shader < model.shader;
			}
			for (unsigned int slot = 0; slot < textures.size() && slot < model.textures.size(); slot++)
			{
				if (slot == textures.size() && slot < model.textures.size())
				{
					return true;
				}
				if (slot == model.textures.size())
				{
					return false;
				}
				if (textures[slot] != model.textures[slot])
				{
					return textures[slot] < model.textures[slot];
				}
			}
			return vertexBufferObject < model.vertexBufferObject;
		}
		else // some blending, sort by depth
		{
			return getDepth() < model.getDepth();
		}
	}

	bool operator < (Ptr<Model> const & lhs, Ptr<Model> const & rhs)
	{
		return *lhs < *rhs;
	}
}