#include "material.hpp"

namespace ve
{
	Material::Material()
	{
	}

	Material::Material(std::string const & filename)
	{

	}

	Material::~Material()
	{
	}

	Ptr<Shader> Material::getShader() const
	{
		return shader;
	}

	void Material::setShader(Ptr<Shader> shader_)
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
					case Uniform::INT: uniform = OwnPtr<UniformInt>::returnNew(pair.first, pair.second.location); break;
					case Uniform::COORD_2I: uniform = OwnPtr<UniformVector2i>::returnNew(pair.first, pair.second.location); break;
					case Uniform::COORD_3I: uniform = OwnPtr<UniformVector3i>::returnNew(pair.first, pair.second.location); break;
					case Uniform::COORD_4I: uniform = OwnPtr<UniformVector4i>::returnNew(pair.first, pair.second.location); break;
					case Uniform::FLOAT: uniform = OwnPtr<UniformFloat>::returnNew(pair.first, pair.second.location); break;
					case Uniform::COORD_2F: uniform = OwnPtr<UniformVector2f>::returnNew(pair.first, pair.second.location); break;
					case Uniform::COORD_3F: uniform = OwnPtr<UniformVector3f>::returnNew(pair.first, pair.second.location); break;
					case Uniform::COORD_4F: uniform = OwnPtr<UniformVector4f>::returnNew(pair.first, pair.second.location); break;
					case Uniform::MATRIX_22F: uniform = OwnPtr<UniformMatrix22f>::returnNew(pair.first, pair.second.location); break;
					case Uniform::MATRIX_33F: uniform = OwnPtr<UniformMatrix33f>::returnNew(pair.first, pair.second.location); break;
					case Uniform::MATRIX_44F: uniform = OwnPtr<UniformMatrix44f>::returnNew(pair.first, pair.second.location); break;
					case Uniform::TEXTURE_2D:
						OwnPtr<UniformTexture2d> sampler2dUniform = OwnPtr<UniformTexture2d>::returnNew(pair.first, pair.second.location, numTextures);
						texture2dUniforms.push_back(sampler2dUniform);
						uniform = sampler2dUniform;
						numTextures++;
						break;
				}
				uniforms[pair.second.location] = uniform;
			}
		}
	}

	Ptr<Uniform> Material::getUniform(int location) const
	{
		auto iter = uniforms.find(location);
		if (iter != uniforms.end())
		{
			return iter->second;
		}
		else
		{
			return Ptr<Uniform>();
		}
	}

	Ptr<Uniform> Material::getUniform(std::string const & name) const
	{
		if (shader)
		{
			return getUniform(shader->getUniformInfo(name).location);
		}
		else
		{
			return Ptr<Uniform>();
		}
	}

	void Material::activate(std::function<void(Material const &)> const & sceneUniformsFunction, std::function<void(Material const &)> const & modelUniformsFunction) const
	{
		if (shader)
		{
			bool newShader = shader->activate();
			if (newShader && sceneUniformsFunction)
			{
				sceneUniformsFunction(*this);
			}
			if (modelUniformsFunction)
			{
				modelUniformsFunction(*this);
			}
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

	bool operator < (Ptr<Material> const & lhs, Ptr<Material> const & rhs)
	{
		if (!lhs.isValid() && rhs.isValid())
		{
			return true;
		}
		if (!rhs.isValid())
		{
			return false;
		}
		return *lhs < *rhs;
	}
}