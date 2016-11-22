#pragma once

#include "gui/sprite.h"
#include "gui/widget_internal.h"
#include "render/model.h"
#include "util/ptr.h"

namespace ve
{
	class SpriteInternal : public virtual Sprite, public virtual WidgetInternal
	{
	public:
		// Constructor.
		SpriteInternal(Ptr<Scene> scene);

		// Destructor.
		~SpriteInternal();

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Loads a image from the store into the sprite.
		void setImage(std::string const & name) override;

		// Updates the sprite (actually does nothing).
		void update(float dt);

	private:
		Recti bounds;
		Ptr<Model> model;
		int minUniformLocation;
		int maxUniformLocation;
		int textureLocation;
	};
}