#include "material.h"

namespace ve
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
	}

	void Material::activate() const
	{
		shader->activate();
		for (auto pair : uniforms)
		{
			pair.second->sendToActiveShader();
		}
	}
}

