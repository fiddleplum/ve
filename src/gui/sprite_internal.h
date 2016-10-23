#pragma once

#include "sprite.h"
#include "widget_internal.h"
#include "../render/texture.h"
#include "ptr.h"

namespace ve
{
	class SpriteInternal : public virtual Sprite, public virtual WidgetInternal
	{
	public:
		// Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Loads a image from the store into the sprite.
		void setImage(std::string const & name) override;

		// Updates the sprite (actually does nothing).
		void update(float dt);

	private:
		Recti bounds;
		UsePtr<Texture> texture;
	};
}