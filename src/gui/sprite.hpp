#pragma once

#include "gui/widget.hpp"
#include "render/model.hpp"
#include "util/ptr.hpp"

namespace ve
{
	class Sprite : public Widget
	{
	public:
		// Constructor.
		Sprite(Ptr<render::Scene> const & scene, Ptr<render::Shader> const & shader);

		// Destructor.
		~Sprite();

		// Internal to gui. Returns the depth.
		float getDepth() const override;

		// Internal to gui. Sets the depth.
		void setDepth(float depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Internal to gui. Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Returns the pixel offset within the image.
		Vector2i getImageOffset() const;

		// Sets the pixel offset within the image.
		void setImageOffset(Vector2i offset);

		// Sets an image for the sprite.
		void setImage(Ptr<render::Image> const & image);

		// Internal to gui. Called when the user moves the cursor within the widget or out of the widget.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition) override;

		// Internal to gui. Updates the sprite (actually does nothing).
		void update(float dt);

	private:
		void updateMesh();

		Recti bounds;
		Vector2i imageOffset;
		Ptr<render::Model> model;
		Ptr<render::Image> image;
		OwnPtr<render::Mesh> mesh;
		int originUniformLocation;
		int imageSizeUniformLocation;
		int imageUniformLocation;
		int colorUniformLocation;
	};
}