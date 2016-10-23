#include "panel_internal.h"
#include "sprite_internal.h"
#include "text_button_internal.h"

namespace ve
{
	UsePtr<Sprite> PanelInternal::createSprite()
	{
		auto widget = OwnPtr<SpriteInternal>::createNew();
		widgets.push_back(widget);
		return widget;
	}

	UsePtr<TextButton> PanelInternal::createTextButton()
	{
		auto widget = OwnPtr<TextButtonInternal>::createNew();
		widgets.push_back(widget);
		return widget;
	}

	void PanelInternal::update(float dt)
	{
		widgets.processElementsToErase();

		for (auto widget : widgets)
		{
			widget->update(dt);
		}
	}

	void PanelInternal::addWidget(OwnPtr<WidgetInternal> widget)
	{
		widgets.push_back(widget);
	}
}