#pragma once

#include "gui/widget.hpp"
#include "gui/sprite.hpp"
#include "render/model.hpp"
#include "util/ptr.hpp"
#include "render/stage.hpp"
#include "world3d/world.hpp"

namespace ve
{
	class Viewport : public Widget
	{
	public:
		// Constructor. This scene is the gui scene, not a world scene.
		Viewport(Ptr<Scene> scene);

		// Destructor.
		~Viewport();

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float & depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Returns the currently used texture stage.
		Ptr<TextureStage> getStage() const;

		// Updates the viewport.
		void update(float dt);

	private:
		OwnPtr<Texture> renderTarget;
		OwnPtr<Sprite> sprite; // Uses much of the functionality of a sprite and more!
		OwnPtr<TextureStage> stage;
	};
}