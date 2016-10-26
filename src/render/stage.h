#pragma once

#include "texture.h"
#include "model.h"
#include "rect.h"
#include "ptr.h"
#include <set>

namespace ve
{
	namespace render
	{
		class Stage
		{
		public:
			// Destructor for inheritance.
			virtual ~Stage() {}

			// Creates a new model in the stage and returns it.
			UsePtr<Model> createModel();

			// Removes a model from the stage.
			void destroyModel(UsePtr<Model> & model);

			// Renders to the target.
			void render() const;

			// Prepares the target surface for rendering.
			virtual void setupTarget() const = 0;

		private:
			std::set<OwnPtr<Model>> models;
		};

		class ScreenStage : public Stage
		{
		public:
			// Sets the window size for calculating the GL bounds.
			void setWindowSize(Vector2i windowSize);

			// Prepares the stage to use the screen as the target.
			void setupTarget() const override;

		private:
			Vector2i windowSize = {0, 0};
		};

		class TextureStage : public Stage
		{
		public:
			// Constructs a new stage. Defaults to rendering to screen.
			TextureStage();

			// Destructs.
			~TextureStage();

			// Clears the targets.
			void clearTargets();

			// Sets a texture as a render target at the specific index.
			void setTarget(int index, UsePtr<Texture> target);

			// Prepares the stage to use the textures as the target.
			void setupTarget() const override;

		private:
			std::vector<UsePtr<Texture>> targets;
			unsigned int framebuffer;
		};
	}
}

