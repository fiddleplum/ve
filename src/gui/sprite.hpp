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
		Sprite(Ptr<render::Scene> scene);

		// Destructor.
		~Sprite();

		// Internal to gui. Returns the depth.
		float getDepth() const override;

		// Internal to gui. Sets the depth.
		void setDepth(float & depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Internal to gui. Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Returns the texture coordinates.
		Vector2i getTextureCoords() const;

		// Sets the texture coordinates.
		void setTextureCoords(Vector2i coords);

		// Loads an image from the store into the sprite.
		void setImage(std::string const & name);

		// Loads a texture from the store into the sprite.
		void setTexture(std::string const & name);

		// Loads a texture into the sprite.
		void setTexture(Ptr<render::Texture> texture);

		// Internal to gui. Called when the user moves the cursor within the widget or out of the widget.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition) override;

		// Internal to gui. Updates the sprite (actually does nothing).
		void update(float dt);

	private:
		void updateVbo();

		Recti bounds;
		Vector2i textureCoords;
		Ptr<render::Model> model;
		Ptr<render::Texture> texture;
		OwnPtr<render::VertexBufferObject> vbo;
		int originUniformLocation;
		int texSizeUniformLocation;
		int texUniformLocation;
		int colorUniformLocation;
	};
}