#pragma once

#include "panel.h"
#include "widget_internal.h"
#include "object_list.h"

namespace ve
{
	class PanelInternal : public virtual Panel, public virtual WidgetInternal
	{
	public:
		UsePtr<Sprite> createSprite() override;

		UsePtr<TextButton> createTextButton() override;

		void update(float dt) override;

	private:
		void addWidget(OwnPtr<WidgetInternal> widget);

		ObjectList<OwnPtr<WidgetInternal>> widgets;
	};
}