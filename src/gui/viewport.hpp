#pragma once

#include "gui/widget.hpp"
#include "gui/sprite.hpp"
#include "render/model.hpp"
#include "util/ptr.hpp"
#include "render/target.hpp"

namespace ve
{
	class Viewport : public Widget
	{
	public:
		// Constructor. This scene is the gui scene, not a world scene.
		Viewport(Ptr<render::Scene> scene);

		// Destructor.
		~Viewport();

		// Internal to gui. Returns the depth.
		float getDepth() const override;

		// Internal to gui. Sets the depth.
		void setDepth(float & depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Internal to gui. Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Returns the texture target that the viewport uses.
		Ptr<render::TextureTarget> getTarget() const;

		// Internal to gui. Called when the user moves the cursor within the widget or out of the widget.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition) override;

		// Internal to gui. Updates the viewport.
		void update(float dt);

	private:
		OwnPtr<render::Texture> renderTexture;
		OwnPtr<Sprite> sprite; // Uses much of the functionality of a sprite and more!
		OwnPtr<render::TextureTarget> target;
	};
}