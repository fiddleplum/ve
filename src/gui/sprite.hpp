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
		Sprite(Ptr<Scene> scene);

		// Destructor.
		~Sprite();

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Sets the bounds of the sprite.
		void setBounds(Recti bounds) override;

		// Returns the texture coordinates.
		Vector2i getTextureCoords() const;

		// Sets the texture coordinates.
		void setTextureCoords(Vector2i coords);

		// Loads a image from the store into the sprite.
		void setImage(std::string const & name);

		// Updates the sprite (actually does nothing).
		void update(float dt);

	private:
		void updateVbo();

		Recti bounds;
		Vector2i textureCoords;
		Ptr<Model> model;
		Ptr<Texture> texture;
		OwnPtr<Mesh> mesh;
		OwnPtr<VertexBufferObject> vbo;
		int originUniformLocation;
		int texSizeUniformLocation;
		int texUniformLocation;
	};
}