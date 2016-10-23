#pragma once

#include "sprite.h"
#include "widget_internal.h"
#include "texture.h"
#include "ptr.h"

namespace ve
{
	class SpriteInternal : public virtual Sprite, public virtual WidgetInternal
	{
	public:
		// Loads a image from the store into the sprite.
		void setImage(std::string const & name) override;

		// Updates the sprite (actually does nothing).
		void update(float dt);

	private:
		UsePtr<Texture> texture;
	};
}

/*
This is a problem. It stores a TextureInternal, but its interface takes a Texture.
There are two solutions:
   1. The setTexture function needs to only take a string. It is obtained internally.
   2. The Texture object needs to have a getInternal() function to get its subclass.

Method two has the problem that then there is an internal function in the API object,
which I've been going to great lengths to avoid. So it's out.

Method one can take a string, which makes it easy to use anyway. I still need a way
for users to create their own images procedurally and put them into the sprite.

The string then can be a name that refers to the image resource store. A user can
load an image into that store or create their own image.

So the resources class would have

UsePtr<Image> getImage(std::string const & name, std::string const & location)
UsePtr<Image> createImage(std::string const & name, ...);

And the resourcesInternal class would have

UsePtr<ImageInternal> getImageInternal(std::string const & name);

Sprite would use the getImageInternal() function, and the user would use the getImage()
and createImage() functions.

*/