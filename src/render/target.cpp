#include "render/target.hpp"
#include "render/open_gl.hpp"
#include <SDL.h>

namespace ve
{
	namespace render
	{
		unsigned int WindowTarget::numWindowTargets = 0;
		void * WindowTarget::glContext = nullptr;

		Target::Target()
		{
		}

		Ptr<Scene> Target::getScene() const
		{
			return scene;
		}

		void Target::setScene(Ptr<Scene> scene_)
		{
			scene = scene_;
		}

		void Target::setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction_)
		{
			uniformsFunction = uniformsFunction_;
		}

		void Target::render() const
		{
			// Clear out the shaders and textures.
			Shader::deactivate();
			Image::deactivateRest(0);

			if (!scene.isValid())
			{
				return;
			}

			for (auto && dependentTarget : scene->getDependentTargets())
			{
				dependentTarget->render();
			}

			preRender();

			glCullFace(GL_BACK);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glClearColor(0, 0, 0, 1);
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			scene->render(uniformsFunction);

			postRender();
		}

		WindowTarget::WindowTarget(void * sdlWindow_)
		{
			sdlWindow = sdlWindow_;

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

			if (numWindowTargets == 0)
			{
				glContext = SDL_GL_CreateContext((SDL_Window *)sdlWindow);
				SDL_GL_MakeCurrent((SDL_Window *)sdlWindow, glContext);
				glInitialize();
			}

			numWindowTargets++;
		}

		WindowTarget::~WindowTarget()
		{
			numWindowTargets--;
			if (numWindowTargets == 0)
			{
				SDL_GL_DeleteContext(glContext);
			}
		}

		Vector2i WindowTarget::getWindowSize() const
		{
			return viewportSize;
		}

		void WindowTarget::setWindowSize(Vector2i size)
		{
			viewportSize = size;
		}

		void WindowTarget::preRender() const
		{
			SDL_GL_MakeCurrent((SDL_Window *)sdlWindow, glContext);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, viewportSize[0], viewportSize[1]);
			glClearColor(0, 0, 0, 1);
		}

		void WindowTarget::postRender() const
		{
			SDL_GL_SwapWindow((SDL_Window *)sdlWindow);
		}

		ImageTarget::ImageTarget()
		{
			glGenFramebuffers(1, &framebuffer);
		}

		ImageTarget::~ImageTarget()
		{
			glDeleteFramebuffers(1, &framebuffer);
		}

		Ptr<Image> ImageTarget::getColorImage(unsigned int index) const
		{
			if (index < colorImages.size())
			{
				return colorImages[index];
			}
			else
			{
				return Ptr<Image>();
			}
		}

		void ImageTarget::setColorImage(unsigned int index, Ptr<Image> image)
		{
			if (index >= colorImages.size())
			{
				colorImages.resize(index + 1);
			}
			colorImages[index] = image;
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			image->attachToFrameBuffer(GL_COLOR_ATTACHMENT0 + index);
			std::vector<unsigned int> indices;
			for (unsigned int i = 0; i < colorImages.size(); i++)
			{
				if (colorImages[i])
				{
					indices.push_back(GL_COLOR_ATTACHMENT0 + i);
				}
				else
				{
					indices.push_back(GL_NONE);
				}
			}
			glDrawBuffers((int)indices.size(), &indices[0]);
		}

		Ptr<Image> ImageTarget::getDepthImage() const
		{
			return depthImage;
		}

		void ImageTarget::setDepthImage(Ptr<Image> image)
		{
			depthImage = image;
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			image->attachToFrameBuffer(GL_DEPTH_ATTACHMENT);
		}

		Ptr<Image> ImageTarget::getStencilImage() const
		{
			return stencilImage;
		}

		void ImageTarget::setStencilImage(Ptr<Image> image)
		{
			stencilImage = image;
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			image->attachToFrameBuffer(GL_STENCIL_ATTACHMENT);
		}

		void ImageTarget::clearAllImages()
		{
			colorImages.clear();
			depthImage.setNull();
			stencilImage.setNull();
		}

		void ImageTarget::preRender() const
		{
			if (colorImages.empty() || !colorImages[0].isValid())
			{
				throw std::runtime_error("The color image at index 0 needs to be valid. ");
			}
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glViewport(0, 0, colorImages[0]->getSize()[0], colorImages[0]->getSize()[1]);
			glClearColor(1, 0, 0, 1);
		}

		void ImageTarget::postRender() const
		{
		}
	}
}