#include "sprite.h"
#include "../render/uniform.h"

namespace ve
{
	namespace gui
	{
		Sprite::Sprite(UsePtr<render::Stage> stage_)
		{
			stage = stage_;
			model = stage->createModel();
			model->setUniformsFunction(std::bind(&Sprite::setUniforms, this));
		}

		Sprite::Sprite(UsePtr<render::Stage> stage_, Config const & config)
			: Sprite(stage_)
		{
			auto textureOpt = config["texture"];
			if (textureOpt && textureOpt->type == Config::String)
			{
				model->getMaterial()->getUniform("texture").as<render::UniformTexture2d>()->texture = render::Texture::cache.get(textureOpt->text);
			}
		}

		Sprite::~Sprite()
		{
			stage->destroyModel(model);
		}

		void Sprite::handleNewDepth()
		{
			model->setDepth(getDepth());
		}

		void Sprite::setTexture(UsePtr<render::Texture> texture_)
		{
			model->getMaterial()->getUniform("texture").as<render::UniformTexture2d>()->texture = texture_;
		}

		bool Sprite::handleInputEvent(InputEvent const & event, std::optional<Vector2i> cursorPosition)
		{
			if (event.is(InputEvent::Type::MouseButton))
			{
				auto mbe = event.as<MouseButtonEvent>();
				if (mbe.button == MouseButtonEvent::Left && cursorIsOver(cursorPosition))
				{
					return true;
				}
			}
			return false;
		}

		void Sprite::setUniforms() const
		{
			Recti const & bounds = getBounds();
			boundsUniform->value = {bounds.min[0], bounds.min[1], bounds.max[0], bounds.max[1]};
		}
	}
}

/*
Regarding Render stage

If I in the constructors I add the model to a stage and have the rendering happen in Gui,
then each element needs to have a z value for sorting purposes, since since rendering isn't
recursively done.

If I have a render function that calls model->render, then it goes around the idea of a stage
rendering everything. That same method won't work for a 3d scene which needs to be sorted
beforehand.

So I can have in the prerenderupdate function a stage->addModelToFrame method which will render
the model in the next frame. the stage can just be set not to do any sorting to let the
recursive behavior take care of the sorting. The naming of the above functions could be better
though.

This method can also work for the 3d scene, each 3d object adding itself to the stage for the
next render. The models already need to be added every frame anyway because of z-sorting issues
with transparent objects and textures/shaders that changed since the last frame.

I can call the function prerender, rather than prerenderUpdate.


It would be nice if the function didn't have to add itself every time. What if I separate the ideas
of the render stage and the render scene?

The sprite would add itself to the render scene on construction. The render scene would simply be a
list of models and maybe other things. The render stage would take the scene as a param to its
render function, sort, and render. No this won't work because of the strict recursive ordering of
the Gui elements (in paragraph 1). I really do need to call the function every time.




*/
