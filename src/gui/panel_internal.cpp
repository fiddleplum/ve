#include "panel_internal.h"
#include "sprite_internal.h"
#include "text_area_internal.h"
#include "text_button_internal.h"
#include <algorithm>

namespace ve
{
	PanelInternal::PanelInternal(Ptr<Scene> scene)
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

	Recti PanelInternal::getBounds() const
	{
		return bounds;
	}

	void PanelInternal::setBounds(Recti bounds_)
	{
		bounds = bounds_;
		for (auto const & widgetInfo : widgetInfos)
		{
			updateWidgetBounds(widgetInfo);
		}
	}

	Ptr<Sprite> PanelInternal::createSprite()
	{
		return createWidget<SpriteInternal>();
	}

	Ptr<TextArea> PanelInternal::createTextArea()
	{
		return createWidget<TextAreaInternal>();
	}

	Ptr<TextButton> PanelInternal::createTextButton()
	{
		return createWidget<TextButtonInternal>();
	}

	void PanelInternal::destroyWidget(Ptr<Widget> widget)
	{
		auto it = getWidgetInfo(widget);
		widgetInfos.queueForErase(it);
	}

	void PanelInternal::setWidgetBounds(Ptr<Widget> widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset)
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
		for (auto const & widgetInfo : widgetInfos)
		{
			widgetInfo.widget->update(dt);
		}

		widgetInfos.processEraseQueue();
	}

	template <typename T> Ptr<T> PanelInternal::createWidget()
	{
		auto widget = OwnPtr<T>::returnNew(getScene());
		WidgetInfo widgetInfo;
		widgetInfo.widget = widget;
		widgetInfos.push_back(widgetInfo);
		updateWidgetBounds(widgetInfo);
		return widget;
	}

	std::list<PanelInternal::WidgetInfo>::iterator PanelInternal::getWidgetInfo(Ptr<Widget> widget)
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
		widgetInfo.widget->setBounds(Recti {Vector2i{widgetPosition}, Vector2i{widgetPosition + widgetSize - Vector2f{1, 1}}});
	}
}