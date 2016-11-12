#include "panel_internal.h"
#include "sprite_internal.h"
#include "text_button_internal.h"
#include <algorithm>

namespace ve
{
	PanelInternal::PanelInternal(UsePtr<Scene> scene)
		: WidgetInternal(scene)
	{

	}

	float PanelInternal::getDepth() const
	{
		return 0;
	}

	void PanelInternal::setDepth(float depth)
	{

	}

	void PanelInternal::setBounds(Recti bounds_)
	{
		bounds = bounds_;
		for (auto const & widgetInfo : widgetInfos)
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

	void PanelInternal::destroyWidget(UsePtr<Widget> widget)
	{
		auto it = getWidgetInfo(widget);
		widgetInfos.erase(it);
	}

	void PanelInternal::setBounds(UsePtr<Widget> widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset)
	{
		auto & widgetInfo = *getWidgetInfo(widget);
		widgetInfo.originInPanel = originInPanel;
		widgetInfo.originInWidget = originInWidget;
		widgetInfo.originOffset = originOffset;
		widgetInfo.sizeInPanel = sizeInPanel;
		widgetInfo.sizeOffset = sizeOffset;
		updateWidgetBounds(widgetInfo);
	}

	void PanelInternal::update(float dt)
	{
		widgetInfos.processElementsToErase();

		for (auto const & widgetInfo : widgetInfos)
		{
			widgetInfo.widget->update(dt);
		}
	}

	template <typename T> UsePtr<T> PanelInternal::createWidget()
	{
		auto widget = OwnPtr<T>::returnNew(getScene());
		WidgetInfo widgetInfo;
		widgetInfo.widget = widget;
		widgetInfos.push_back(widgetInfo);
		updateWidgetBounds(widgetInfo);
		return widget;
	}

	std::vector<PanelInternal::WidgetInfo>::iterator PanelInternal::getWidgetInfo(UsePtr<Widget> widget)
	{
		for (auto it = widgetInfos.begin(); it != widgetInfos.end(); it++)
		{
			if (it->widget == widget)
			{
				return it;
			}
		}
		throw std::runtime_error("Widget not found. ");
	}

	void PanelInternal::updateWidgetBounds(WidgetInfo const & widgetInfo) const
	{
		Vector2f panelSize = Vector2f {bounds.max - bounds.min + Vector2i {1, 1}};
		Vector2f widgetSize = panelSize.scale(widgetInfo.sizeInPanel) + Vector2f {widgetInfo.sizeOffset};
		Vector2f widgetPosition = panelSize.scale(widgetInfo.originInPanel) - widgetSize.scale(widgetInfo.originInWidget) + Vector2f {widgetInfo.originOffset};
		widgetInfo.widget->setBounds(Recti {Vector2i{widgetPosition}, Vector2i{widgetPosition + widgetSize}});
	}
}