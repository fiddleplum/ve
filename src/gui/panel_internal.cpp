#include "panel_internal.h"
#include "sprite_internal.h"
#include "text_button_internal.h"

namespace ve
{
	void PanelInternal::setBounds(Recti bounds_)
	{
		bounds = bounds_;
		for (auto widgetInfo : widgetInfos)
		{
			updateWidgetBounds(widgetInfo);
		}
	}

	UsePtr<Sprite> PanelInternal::createSprite()
	{
		return createWidget<SpriteInternal>();
	}

	UsePtr<TextButton> PanelInternal::createTextButton()
	{
		return createWidget<TextButtonInternal>();
	}

	void PanelInternal::setBounds(UsePtr<Widget> widget, Coord2f originInPanel, Coord2f originInWidget, Coord2i originOffset, Coord2f sizeInPanel, Coord2i sizeOffset)
	{
		for (auto it = widgetInfos.begin(); it != widgetInfos.end(); it++)
		{
			if (it->widget == widget)
			{
				auto & widgetInfo = *it;
				widgetInfo.originInPanel = originInPanel;
				widgetInfo.originInWidget = originInWidget;
				widgetInfo.originOffset = originOffset;
				widgetInfo.sizeInPanel = sizeInPanel;
				widgetInfo.sizeOffset = sizeOffset;
				updateWidgetBounds(widgetInfo);
				return;
			}
		}
		throw std::runtime_error("Widget not found. ");
	}

	void PanelInternal::update(float dt)
	{
		widgetInfos.processElementsToErase();

		for (auto widgetInfo : widgetInfos)
		{
			widgetInfo.widget->update(dt);
		}
	}

	template <typename T> UsePtr<T> PanelInternal::createWidget()
	{
		OwnPtr<T> widget = OwnPtr<T>::createNew();
		WidgetInfo widgetInfo;
		widgetInfo.widget = widget;
		widgetInfos.push_back(widgetInfo);
		updateWidgetBounds(widgetInfo);
		return widget;
	}

	void PanelInternal::updateWidgetBounds(WidgetInfo const & widgetInfo) const
	{
		Coord2f panelSize = Coord2f {bounds.max - bounds.min + Coord2i {1, 1}};
		Coord2f widgetSize = panelSize.scale(widgetInfo.sizeInPanel) + Coord2f {widgetInfo.sizeOffset};
		Coord2f widgetPosition = panelSize.scale(widgetInfo.originInPanel) - widgetSize.scale(widgetInfo.originInWidget) + Coord2f {widgetInfo.originOffset};
		widgetInfo.widget->setBounds(Recti {Coord2i{widgetPosition}, Coord2i{widgetPosition + widgetSize}});
	}
}