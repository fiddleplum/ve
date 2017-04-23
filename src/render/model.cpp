#include "render/model.hpp"

namespace ve
{
	namespace render
	{
		Model::Model()
		{
		}

		float Model::getDepth() const
		{
			return depth;
		}

		void Model::setDepth(float depth_)
		{
			depth = depth_;
		}

		Ptr<Mesh> Model::getMesh() const
		{
			return mesh;
		}

		void Model::setMesh(Ptr<Mesh> const & mesh_)
		{
			mesh = mesh_;
		}

		Ptr<Shader> Model::getShader() const
		{
			return shader;
		}

		void Model::setShader(Ptr<Shader> const & shader_)
		{
			shader = shader_;
		}

		Ptr<Image> Model::getImageAtSlot(unsigned int slot) const
		{
			if (slot >= images.size())
			{
				return Ptr<Image>();
			}
			else
			{
				return images[slot];
			}
		}

		void Model::setImageAtSlot(Ptr<Image> const & image, unsigned int slot)
		{
			if (slot >= images.size())
			{
				images.resize(slot + 1);
			}
			images[slot] = image;
		}

		void Model::setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction_)
		{
			uniformsFunction = uniformsFunction_;
		}

		void Model::render(std::function<void(Ptr<Shader> const &)> const & stageUniformsFunction, std::function<void(Ptr<Shader> const &)> const & sceneUniformsFunction, bool flipY) const
		{
			if (!shader || !mesh)
			{
				return;
			}
			bool newShader = shader->activate();
			if (newShader)
			{
				if (stageUniformsFunction)
				{
					stageUniformsFunction(shader);
				}
				if (sceneUniformsFunction)
				{
					sceneUniformsFunction(shader);
				}
				shader->setUniformValue<float>("flipY", flipY ? -1.0f : 1.0f);
			}
			if (uniformsFunction)
			{
				uniformsFunction(shader);
			}
			for (unsigned int slot = 0; slot < images.size(); slot++)
			{
				if (images[slot].isValid())
				{
					images[slot]->activate(slot);
				}
			}
			Image::deactivateRest((unsigned int)images.size());
			mesh->render();
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
				for (unsigned int slot = 0; slot < images.size() && slot < model.images.size(); slot++)
				{
					if (slot == images.size() && slot < model.images.size())
					{
						return true;
					}
					if (slot == model.images.size())
					{
						return false;
					}
					if (images[slot] != model.images[slot])
					{
						return images[slot] < model.images[slot];
					}
				}
				return mesh < model.mesh;
			}
			else if (getDepth() < model.getDepth()) // some blending, sort by depth
			{
				return true;
			}
			else if (getDepth() > model.getDepth())
			{
				return false;
			}
			else // just do the pointer to guarantee an explicit ordering.
			{
				return this < &model;
			}
		}

		bool operator < (Ptr<Model> const & lhs, Ptr<Model> const & rhs)
		{
			return *lhs < *rhs;
		}
	}
}