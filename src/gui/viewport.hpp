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
		Viewport(Ptr<Stage> guiStage, Ptr<Scene> scene);

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

		// Sets the currently used texture stage.
		void setWorld(Ptr<world3d::World> const & world);

		// Updates the viewport.
		void update(float dt);

	private:
		void updateVbo();
		OwnPtr<Sprite> sprite; // Uses much of the functionality of a sprite and more!
		OwnPtr<TextureStage> stage;
		Ptr<Stage> guiStage;
		Ptr<world3d::World> world;
	};
}