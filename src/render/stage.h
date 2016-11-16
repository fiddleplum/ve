#pragma once

#include "render/texture.h"
#include "render/scene.h"
#include "util/rect.h"
#include "util/ptr.h"
#include <set>

namespace ve
{
	class Stage
	{
	public:
		// Destructor for inheritance.
		virtual ~Stage() {}

		// Adds a stage that this stage depends on.
		void addPriorStage(UsePtr<Stage> stage);

		// Removes a stage that this no longer depends on.
		void removePriorStage(UsePtr<Stage> stage);

		// Clears all stages that this stage depends on.
		void clearPriorStages();

		// Sets the scene that this stage will render.
		void setScene(UsePtr<Scene> scene);

		// Renders the scene to the target. First renders all unrendered prior stages.
		void render() const;

	protected:
		// Prepares the target surface for rendering.
		virtual void setupTarget() const = 0;

	private:
		std::set<UsePtr<Stage>> priorStages;
		UsePtr<Scene> scene;
	};

	class WindowStage : public Stage
	{
	public:
		// Sets the window size for calculating the GL bounds.
		void setWindowSize(Vector2i windowSize);

	protected:
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

	protected:
		// Prepares the stage to use the textures as the target.
		void setupTarget() const override;

	private:
		std::vector<UsePtr<Texture>> targets;
		unsigned int framebuffer;
	};
}