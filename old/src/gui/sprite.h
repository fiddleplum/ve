#pragma once

#include "element.h"
#include "../render/stage.h"
#include "../config.h"

namespace ve
{
	namespace gui
	{
		class Sprite : public Element
		{
		public:
			Sprite(UsePtr<render::Stage> stage);

			Sprite(UsePtr<render::Stage> stage, Config const & config);

			~Sprite();

			void handleNewBounds() override;

			void handleNewDepth() override;

			void setTexture(UsePtr<render::Texture> texture);

			bool handleInputEvent(InputEvent const & event, std::optional<Vector2i> cursorPosition) override;

		private:
			void setUniforms() const;

			UsePtr<render::Stage> stage;
			UsePtr<render::Model> model;
			UsePtr<render::UniformVector4i> boundsUniform;
		};
	}
}

