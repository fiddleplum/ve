#pragma once

#include "render/texture.hpp"
#include "render/scene.hpp"
#include "util/rect.hpp"
#include "util/ptr.hpp"
#include <set>

namespace ve
{
	namespace render
	{
		class Target
		{
		public:
			//! Constructor.
			Target();

			//! Virtual default destructor for inheritance.
			virtual ~Target() {}

			//! Returns the scene that will be rendered to this target.
			Ptr<Scene> getScene() const;

			//! Sets the scene that will be rendered to this target.
			void setScene(Ptr<Scene> scene);

			//! Sets a function to be called that sets any target-specific uniforms. Called every time the shader is changed.
			void setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction);

			//! Renders the scene to the target. First renders all targets upon which the scene depends.
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

		class WindowTarget : public Target
		{
		public:
			//! Constructs the window stage.
			WindowTarget(void * sdlWindow);

			//! Destructs the window stage.
			~WindowTarget();

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
			static unsigned int numWindowTargets;
			static void * glContext;
			void * sdlWindow;
			Vector2i viewportSize;
		};

		class TextureTarget : public Target
		{
		public:
			// Constructor. If window is true, this stage is rendering directly to the window.
			TextureTarget();

			// Destructor.
			~TextureTarget();

			// Returns the texture at the specified index.
			Ptr<Texture> getColorTexture(unsigned int index) const;

			// Sets a color texture at the specified index.
			void setColorTexture(unsigned int index, Ptr<Texture> target);

			// Returns the depth texture.
			Ptr<Texture> getDepthTexture() const;

			// Sets a depth texture.
			void setDepthTexture(Ptr<Texture> target);

			// Returns the stencil texture.
			Ptr<Texture> getStencilTexture() const;

			// Sets a stencil texture.
			void setStencilTexture(Ptr<Texture> target);

			// Clears the color, depth and stencil textures.
			void clearAllTextures();

		protected:
			// Called just before rendering the scene.
			void preRender() const override;

			// Called just after rendering the scene.
			void postRender() const override;

		private:
			std::vector<Ptr<Texture>> colorTextures;
			Ptr<Texture> depthTexture;
			Ptr<Texture> stencilTexture;
			unsigned int framebuffer;
		};
	}
}