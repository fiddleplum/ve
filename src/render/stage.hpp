#pragma once

#include "render/texture.hpp"
#include "render/scene.hpp"
#include "util/rect.hpp"
#include "util/ptr.hpp"
#include <set>

namespace ve
{
	class GL;

	class Stage
	{
	public:
		//! Constructor.
		Stage();

		//! Virtual default destructor for inheritance.
		virtual ~Stage() {}

		//! Returns the scene that this stage will render.
		Ptr<Scene> getScene() const;

		//! Sets the scene that this stage will render.
		void setScene(Ptr<Scene> scene);

		//! Adds a function to be called that sets any stage-specific uniforms. Called every time the shader is changed.
		void setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction);

		//! Renders the scene to the target. First renders all unrendered prior stages.
		void render() const;

	protected:
		// Called just before rendering the scene.
		virtual void preRender() const = 0;

		// Called just after rendering the scene.
		virtual void postRender() const = 0;

	private:
		Ptr<Scene> scene;
		std::function<void(Ptr<Shader> const &)> uniformsFunction;
	};

	class WindowStage : public Stage
	{
	public:
		//! Constructs the window stage.
		WindowStage(void * sdlWindow);

		//! Destructs the window stage.
		~WindowStage();

		//! Returns the window used in rendering.
		Vector2i getWindowSize() const;

		//! Sets the window used in rendering.
		void setWindowSize(Vector2i size);

	protected:
		// Called just before rendering the scene.
		void preRender() const override;

		// Called just after rendering the scene.
		void postRender() const override;

	private:
		static unsigned int numWindowStages;
		static void * glContext;
		void * sdlWindow;
		Vector2i viewportSize;
		OwnPtr<GL> gl;
	};

	class TextureStage : public Stage
	{
	public:
		// Constructor. If window is true, this stage is rendering directly to the window.
		TextureStage();

		// Destructor.
		~TextureStage();

		// Returns the texture at the specified index.
		Ptr<Texture> getColorTarget(unsigned int index) const;

		// Sets a texture as a color render target at the specified index.
		void setColorTarget(unsigned int index, Ptr<Texture> target);

		// Returns the depth texture render target.
		Ptr<Texture> getDepthTarget() const;

		// Sets a texture as a depth render target.
		void setDepthTarget(Ptr<Texture> target);

		// Returns the stencil texture render target.
		Ptr<Texture> getStencilTarget() const;

		// Sets a texture as a stencil render target.
		void setStencilTarget(Ptr<Texture> target);

		// Clears the color, depth and stencil targets.
		void clearAllTargets();

	protected:
		// Called just before rendering the scene.
		void preRender() const override;

		// Called just after rendering the scene.
		void postRender() const override;

	private:
		std::vector<Ptr<Texture>> colorTargets;
		Ptr<Texture> depthTarget;
		Ptr<Texture> stencilTarget;
		unsigned int framebuffer;
	};
}