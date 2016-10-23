#include "sprite_internal.h"
#include "app_internal.h"

namespace ve
{
	void SpriteInternal::setImage(std::string const & name)
	{
		auto textureName = name + "_texture";
		auto store = getAppInternal()->getResourceStoreInternal();
		texture = store->getTexture(textureName);
		if (!texture)
		{
			auto image = store->getImage(name);
			if (!image)
			{
				throw std::runtime_error("Image name '" + name + "' not found. ");
			}
			store->createTexture(textureName, image);
			texture = store->getTexture(textureName);
		}
	}

	void SpriteInternal::update(float dt)
	{

	}
}