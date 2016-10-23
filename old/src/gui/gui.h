#pragma once

#include "container.h"

namespace ve
{
	namespace gui
	{
		class Gui : public Container
		{
		public:
			Gui();
			~Gui();

		private:
			UsePtr<render::Model> rectModel;
			OwnPtr<render::Stage> stage;
		};
	}
}

