#include "gui.h"
#include "sprite.h"

namespace ve
{
	namespace gui
	{
		Gui::Gui()
		{
			stage.setNew();
			rectModel = stage->createModel();
			rectModel->setMaterial(render::Material::cache
		}

		Gui::~Gui()
		{
		}
	}
}

