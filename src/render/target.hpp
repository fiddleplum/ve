#pragma once

#include "render/image.hpp"
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

			//! Returns the pixel size of the render target and the associated images.
			Vector2i getSize() const;

			//! Sets the pixel size of the render target and the associated images.
			virtual void setSize(Vector2i size);

			//! Returns the scene that will be rendered to this target.
			Ptr<Scene> getScene() const;

			//! Sets the scene that will be rendered to this target.
			virtual void setScene(Ptr<Scene> scene);

			//! Sets a function to be called that sets any target-specific uniforms. Called every time the shader is changed.
			virtual void setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction);

			//! Clears the renderedThisFrame flag for proper scene/target dependency graph travel.
			void clearRenderedThisFrameFlag();

			//! Renders the scene to the target. First renders all targets upon which the scene depends.
			void render() const;

			// Called just before rendering the scene.
			virtual void preRender() const = 0;

			// Called just after rendering the scene.
			virtual void postRender() const = 0;

			// Windows renders to image targets (frame buffers) upside-down, and so this is required to render things rightside-up.
			bool flipY;

		private:
			Vector2i size;
			Ptr<Scene> scene;
			std::function<void(Ptr<Shader> const &)> uniformsFunction;
			mutable bool renderedThisFrame;
		};

		class WindowTarget : public Target
		{
		public:
			//! Constructs the window stage.
			WindowTarget(void * sdlWindow);

			//! Destructs the window stage.
			~WindowTarget();

		protected:
			// Called just before rendering the scene.
			void preRender() const override;

			// Called just after rendering the scene.
			void postRender() const override;

		private:
			static unsigned int numWindowTargets;
			static void * glContext;
			void * sdlWindow;
		};

		// An target that writes the output to images.
		class ImageTarget : public Target
		{
		public:
			// Constructor. If window is true, this stage is rendering directly to the window.
			ImageTarget();

			// Destructor.
			~ImageTarget();

			//! Sets the pixel size of the render target and the associated images.
			void setSize(Vector2i size) override;

			// Returns the image at the specified index.
			Ptr<Image> getColorImage(unsigned int index) const;

			// Sets a color image at the specified index.
			void setColorImage(unsigned int index, Ptr<Image> target);

			// Returns the depth image.
			Ptr<Image> getDepthImage() const;

			// Sets a depth image.
			void setDepthImage(Ptr<Image> target);

			// Returns the stencil image.
			Ptr<Image> getStencilImage() const;

			// Sets a stencil image.
			void setStencilImage(Ptr<Image> target);

			// Clears the color, depth and stencil images.
			void clearAllImages();

		protected:
			// Called just before rendering the scene.
			void preRender() const override;

			// Called just after rendering the scene.
			void postRender() const override;

		private:
			std::vector<Ptr<Image>> colorImages;
			Ptr<Image> depthImage;
			Ptr<Image> stencilImage;
			unsigned int framebuffer;
		};
	}
}