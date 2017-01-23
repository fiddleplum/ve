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
			if (!scene.isValid())
			{
				return;
			}

			for (auto && dependentTarget : scene->getDependentTargets())
			{
				dependentTarget->render();
			}

			preRender();

			//glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			//glEnable(GL_TEXTURE_2D);
			//glDisable(GL_BLEND);
			glClearColor(0, 0, 0, 1);
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
		}

		void WindowTarget::postRender() const
		{
			SDL_GL_SwapWindow((SDL_Window *)sdlWindow);
		}

		TextureTarget::TextureTarget()
		{
			glGenFramebuffers(1, &framebuffer);
		}

		TextureTarget::~TextureTarget()
		{
			glDeleteFramebuffers(1, &framebuffer);
		}

		Ptr<Texture> TextureTarget::getColorTexture(unsigned int index) const
		{
			if (index < colorTextures.size())
			{
				return colorTextures[index];
			}
			else
			{
				return Ptr<Texture>();
			}
		}

		void TextureTarget::setColorTexture(unsigned int index, Ptr<Texture> texture)
		{
			if (index >= colorTextures.size())
			{
				colorTextures.resize(index + 1);
			}
			colorTextures[index] = texture;
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture->getGLId(), 0);
			std::vector<unsigned int> indices;
			for (unsigned int i = 0; i < colorTextures.size(); i++)
			{
				if (colorTextures[i])
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

		Ptr<Texture> TextureTarget::getDepthTexture() const
		{
			return depthTexture;
		}

		void TextureTarget::setDepthTexture(Ptr<Texture> texture)
		{
			depthTexture = texture;
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getGLId(), 0);
		}

		Ptr<Texture> TextureTarget::getStencilTexture() const
		{
			return stencilTexture;
		}

		void TextureTarget::setStencilTexture(Ptr<Texture> texture)
		{
			stencilTexture = texture;
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, texture->getGLId(), 0);
		}

		void TextureTarget::clearAllTextures()
		{
			colorTextures.clear();
			depthTexture.setNull();
			stencilTexture.setNull();
		}

		void TextureTarget::preRender() const
		{
			if (colorTextures.empty() || !colorTextures[0].isValid())
			{
				throw std::runtime_error("The color texture at index 0 needs to be valid. ");
			}
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glViewport(0, 0, colorTextures[0]->getSize()[0], colorTextures[0]->getSize()[1]);
		}

		void TextureTarget::postRender() const
		{
		}
	}
}