#include "render/target.hpp"
#include "render/open_gl.hpp"
#include <log.hpp>
#include <SDL.h>

namespace ve
{
	namespace render
	{
		unsigned int WindowTarget::numWindowTargets = 0;
		void * WindowTarget::glContext = nullptr;

		Target::Target()
		{
			flipY = false;
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

		void Target::clearRenderedThisFrameFlag()
		{
			renderedThisFrame = false;
			if (scene.isValid())
			{
				for (auto && dependentTarget : scene->getDependentTargets())
				{
					if (dependentTarget->renderedThisFrame)
					{
						dependentTarget->clearRenderedThisFrameFlag();
					}
				}
			}
		}

		void Target::render() const
		{
			if (!scene.isValid())
			{
				return;
			}

			// Clear out the shaders and textures.
			Shader::deactivate();
			Image::deactivateRest(0);

			for (auto && dependentTarget : scene->getDependentTargets())
			{
				if (!dependentTarget->renderedThisFrame)
				{
					dependentTarget->render();
				}
			}

			preRender();

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glClearColor(0, 0, 0, 1);
			glClearDepth(1.0);
			glViewport(0, 0, getSize()[0], getSize()[1]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			scene->render(uniformsFunction, flipY);

			postRender();

			renderedThisFrame = true;
		}

		WindowTarget::WindowTarget(void * sdlWindow_)
		{
			sdlWindow = sdlWindow_;

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

			if (numWindowTargets == 0)
			{
				glContext = SDL_GL_CreateContext((SDL_Window *)sdlWindow);
				SDL_GL_MakeCurrent((SDL_Window *)sdlWindow, glContext);
				glInitialize();
				Log::write((char const *)glGetString(GL_VERSION));
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

		Vector2i WindowTarget::getSize() const
		{
			Vector2i size;
			SDL_GetWindowSize((SDL_Window *)sdlWindow, &size[0], &size[1]);
			return size;
		}

		void WindowTarget::preRender() const
		{
			SDL_GL_MakeCurrent((SDL_Window *)sdlWindow, glContext);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(1, 0, 0, 1);
		}

		void WindowTarget::postRender() const
		{
			SDL_GL_SwapWindow((SDL_Window *)sdlWindow);
		}

		ImageTarget::ImageTarget()
		{
			if (std::string(SDL_GetPlatform()) == "Windows") // Windows renders to targets upside-down, so the shaders need to flip everything on the Y-axis.
			{
				flipY = true;
			}
			glGenFramebuffers(1, &framebuffer);
		}

		ImageTarget::~ImageTarget()
		{
			glDeleteFramebuffers(1, &framebuffer);
		}

		Vector2i ImageTarget::getSize() const
		{
			if (colorImages.size() > 0)
			{
				return colorImages[0]->getSize();
			}
			if (depthImage.isValid())
			{
				return depthImage->getSize();
			}
			if (stencilImage.isValid())
			{
				return stencilImage->getSize();
			}
			return Vector2i {0, 0};
		}

		void ImageTarget::setSize(Vector2i size)
		{
			for (unsigned int i = 0; i < colorImages.size(); i++)
			{
				colorImages[i]->setSize(size);
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				colorImages[i]->attachToFrameBuffer(GL_COLOR_ATTACHMENT0 + i);
			}
			if (depthImage.isValid())
			{
				depthImage->setSize(size);
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				depthImage->attachToFrameBuffer(GL_DEPTH_ATTACHMENT);
			}
			if (stencilImage.isValid())
			{
				stencilImage->setSize(size);
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				stencilImage->attachToFrameBuffer(GL_STENCIL_ATTACHMENT);
			}
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
			image->setSize(getSize());
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
			image->setSize(getSize());
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
			image->setSize(getSize());
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
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glClearColor(0, 0, 1, 1);
		}

		void ImageTarget::postRender() const
		{
		}
	}
}