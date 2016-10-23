#include "material.h"

namespace ve
{
	namespace render
	{
		Material::Material()
		{
		}

		Material::~Material()
		{
		}

		UsePtr<Shader> Material::getShader() const
		{
			return shader;
		}

		void Material::setShader(UsePtr<Shader> shader_)
		{
			shader = shader_;
			uniforms.clear();
			texture2dUniforms.clear();
			numTextures = 0;
			if (shader)
			{
				auto uniformInfos = shader->getUniformInfos();
				OwnPtr<Uniform> uniform;
				for (auto pair : uniformInfos)
				{
					switch (pair.second.type)
					{
						case Uniform::INT: uniform = OwnPtr<UniformInt>::createNew(pair.second.location); break;
						case Uniform::VECTOR_2I: uniform = OwnPtr<UniformVector2i>::createNew(pair.second.location); break;
						case Uniform::VECTOR_3I: uniform = OwnPtr<UniformVector3i>::createNew(pair.second.location); break;
						case Uniform::VECTOR_4I: uniform = OwnPtr<UniformVector4i>::createNew(pair.second.location); break;
						case Uniform::FLOAT: uniform = OwnPtr<UniformFloat>::createNew(pair.second.location); break;
						case Uniform::VECTOR_2F: uniform = OwnPtr<UniformVector2f>::createNew(pair.second.location); break;
						case Uniform::VECTOR_3F: uniform = OwnPtr<UniformVector3f>::createNew(pair.second.location); break;
						case Uniform::VECTOR_4F: uniform = OwnPtr<UniformVector4f>::createNew(pair.second.location); break;
						case Uniform::MATRIX_22F: uniform = OwnPtr<UniformMatrix22f>::createNew(pair.second.location); break;
						case Uniform::MATRIX_33F: uniform = OwnPtr<UniformMatrix33f>::createNew(pair.second.location); break;
						case Uniform::MATRIX_44F: uniform = OwnPtr<UniformMatrix44f>::createNew(pair.second.location); break;
						case Uniform::TEXTURE_2D:
							OwnPtr<UniformTexture2d> sampler2dUniform = OwnPtr<UniformTexture2d>::createNew(pair.second.location, numTextures);
							texture2dUniforms.push_back(sampler2dUniform);
							uniform = sampler2dUniform;
							numTextures++;
							break;
					}
					uniforms[pair.second.location] = uniform;
				}
			}
		}

		UsePtr<Uniform> Material::getUniform(int location)
		{
			auto iter = uniforms.find(location);
			if (iter != uniforms.end())
			{
				return iter->second;
			}
			else
			{
				return UsePtr<Uniform>();
			}
		}

		UsePtr<Uniform> Material::getUniform(std::string const & name)
		{
			if (shader)
			{
				return getUniform(shader->getUniformInfo(name).location);
			}
			else
			{
				return UsePtr<Uniform>();
			}
		}

		void Material::activate() const
		{
			if (shader)
			{
				bool newShader = shader->activate();
				Texture::deactivateRest(numTextures);
				for (auto pair : uniforms)
				{
					pair.second->sendToActiveShader();
				}
			}
		}

		bool Material::operator < (Material const & material) const
		{
			if (shader < material.shader)
				return true;
			if (shader > material.shader)
				return false;
			int i = 0;
			while (true)
			{
				if (i >= texture2dUniforms.size())
				{
					if (i >= material.texture2dUniforms.size())
						return false;
					else
						return true;
				}
				if (i >= material.texture2dUniforms.size())
					return false;
				if (texture2dUniforms[i]->texture < material.texture2dUniforms[i]->texture)
					return true;
				if (texture2dUniforms[i]->texture > material.texture2dUniforms[i]->texture)
					return false;
				i++;
			}
		}

		bool operator < (UsePtr<Material> const & lhs, UsePtr<Material> const & rhs)
		{
			return *lhs < *rhs;
		}
	}
}

