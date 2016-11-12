#include "stage.h"
#include "open_gl.h"

namespace ve
{
	void Stage::addPriorStage(UsePtr<Stage> stage)
	{
		priorStages.insert(stage);
	}

	void Stage::removePriorStage(UsePtr<Stage> stage)
	{
		priorStages.erase(stage);
	}

	void Stage::clearPriorStages()
	{
		priorStages.clear();
	}

	void Stage::setScene(UsePtr<Scene> scene_)
	{
		scene = scene_;
	}

	void Stage::render() const
	{
		setupTarget();

		scene->render();
	}

	void WindowStage::setWindowSize(Vector2i windowSize_)
	{
		windowSize = windowSize_;
	}

	void WindowStage::setupTarget() const
	{
		glViewport(0, 0, windowSize[0], windowSize[1]);
	}

	TextureStage::TextureStage()
	{
		glGenFramebuffers(1, &framebuffer);
	}

	TextureStage::~TextureStage()
	{
		glDeleteFramebuffers(1, &framebuffer);
	}

	void TextureStage::clearTargets()
	{
		glDeleteFramebuffers(1, &framebuffer);
		glGenFramebuffers(1, &framebuffer);
	}

	void TextureStage::setTarget(int index, UsePtr<Texture> target)
	{
		assert(0 <= index);
		if (index >= targets.size())
		{
			targets.resize(index + 1);
		}
		targets[index] = target;
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target->getGLId(), 0);
		std::vector<unsigned int> indices;
		for (int i = 0; i < targets.size(); i++)
		{
			if (targets[i])
			{
				indices[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			else
			{
				indices[i] = GL_NONE;
			}
		}
		glDrawBuffers((int)indices.size(), &indices[0]);
	}

	void TextureStage::setupTarget() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		if (!targets.empty())
		{
			Vector2i size = targets[0]->getSize(); // Use the first target as the viewport size. All targets should be the same size.
			glViewport(0, 0, size[0], size[1]);
		}
	}
}