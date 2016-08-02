#pragma once

#include "element.h"
#include "../ptr.h"
#include "../scene/camera.h"

namespace ve
{
	namespace gui
	{
		class Viewport : public Element
		{
		public:
			virtual Recti getBounds() const;

			virtual void setPosition(Vector2i position);

			virtual void setSize(Vector2i size);

			virtual void render(Vector2i windowSize) const;

		private:
			Recti bounds;
			UsePtr<scene::Camera> camera;
		};
	}
}

