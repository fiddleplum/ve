#pragma once

namespace ve
{
	class ResourceManager
	{
	public:
		UsePtr<Texture> getTexture(std::string const & filename);
	};
}

