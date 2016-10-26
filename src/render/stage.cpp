#include "stage.h"
#include "open_gl.h"

namespace ve
{
	namespace render
	{
		UsePtr<Model> Stage::createModel()
		{
			OwnPtr<Model> model = OwnPtr<Model>::createNew();
			models.insert(model);
			return model;
		}

		void Stage::destroyModel(UsePtr<Model> & model)
		{
			auto iter = std::find(models.begin(), models.end(), model);
			if (iter != models.end())
			{
				models.erase(iter);
				model.setNull();
			}
			else
			{
				throw std::runtime_error("Model not found. ");
			}
		}

		void Stage::render() const
		{
			setupTarget();

			std::set<UsePtr<Model>> modelsSorted;
			for (auto model : models)
			{
				modelsSorted.insert(model);
			}

			for (auto model : modelsSorted)
			{
				model->render();
			}
		}

		void ScreenStage::setWindowSize(Vector2i windowSize_)
		{
			windowSize = windowSize_;
		}

		void ScreenStage::setupTarget() const
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
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target->id, 0);
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
}

