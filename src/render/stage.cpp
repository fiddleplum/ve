#include "stage.hpp"
#include "open_gl.hpp"

namespace ve
{
	Stage::Stage()
	{
	}

	Ptr<Scene> Stage::getScene() const
	{
		return scene;
	}

	void Stage::setScene(Ptr<Scene> scene_)
	{
		scene = scene_;
	}

	void Stage::setUniformsFunction(std::function<void(Ptr<Shader> const &)> const & uniformsFunction_)
	{
		uniformsFunction = uniformsFunction_;
	}

	void Stage::render()
	{
		if (!scene.isValid())
		{
			return;
		}

		for (auto && dependentStage : scene->getDependentStages())
		{
			dependentStage->render();
		}

		setupTarget();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glClearColor(0, 0, 0, 1);
		glClearDepth(-1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->render(uniformsFunction);
	}

	Vector2i WindowStage::getWindowSize() const
	{
		return viewportSize;
	}

	void WindowStage::setWindowSize(Vector2i size)
	{
		viewportSize = size;
	}

	void WindowStage::setupTarget() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, viewportSize[0], viewportSize[1]);
	}

	TextureStage::TextureStage()
	{
		glGenFramebuffers(1, &framebuffer);
	}

	TextureStage::~TextureStage()
	{
		glDeleteFramebuffers(1, &framebuffer);
	}

	Ptr<Texture> TextureStage::getColorTarget(unsigned int index) const
	{
		if (index < colorTargets.size())
		{
			return colorTargets[index];
		}
		else
		{
			return Ptr<Texture>();
		}
	}

	void TextureStage::setColorTarget(unsigned int index, Ptr<Texture> target)
	{
		if (index >= colorTargets.size())
		{
			colorTargets.resize(index + 1);
		}
		colorTargets[index] = target;
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target->getGLId(), 0);
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < colorTargets.size(); i++)
		{
			if (colorTargets[i])
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

	Ptr<Texture> TextureStage::getDepthTarget() const
	{
		return depthTarget;
	}

	void TextureStage::setDepthTarget(Ptr<Texture> target)
	{
		depthTarget = target;
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target->getGLId(), 0);
	}

	Ptr<Texture> TextureStage::getStencilTarget() const
	{
		return stencilTarget;
	}

	void TextureStage::setStencilTarget(Ptr<Texture> target)
	{
		stencilTarget = target;
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, target->getGLId(), 0);
	}

	void TextureStage::clearAllTargets()
	{
		colorTargets.clear();
		depthTarget.setNull();
		stencilTarget.setNull();
	}

	void TextureStage::setupTarget() const
	{
		if (colorTargets.empty() || !colorTargets[0].isValid())
		{
			throw std::runtime_error("The color target at index 0 needs to be valid. ");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, colorTargets[0]->getSize()[0], colorTargets[0]->getSize()[1]);
	}
}