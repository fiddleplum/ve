#include "panel.hpp"
#include <algorithm>

namespace ve
{
	Panel::Panel(Ptr<Scene> scene)
		: Widget(scene)
	{

	}

	float Panel::getDepth() const
	{
		return 0;
	}

	void Panel::setDepth(float & depth_)
	{
		depth = depth_;
		for (auto const & widgetInfo : widgetInfos)
		{
			widgetInfo.widget->setDepth(depth_);
		}
		depth_++;
	}

	Recti Panel::getBounds() const
	{
		return bounds;
	}

	void Panel::setBounds(Recti bounds_)
	{
		bounds = bounds_;
		for (auto const & widgetInfo : widgetInfos)
		{
			updateWidgetBounds(widgetInfo);
		}
	}

	Ptr<Sprite> Panel::createSprite()
	{
		return createWidget<Sprite>();
	}

	Ptr<TextArea> Panel::createTextArea()
	{
		return createWidget<TextArea>();
	}

	Ptr<TextButton> Panel::createTextButton()
	{
		return createWidget<TextButton>();
	}

	Ptr<Viewport> Panel::createViewport()
	{
		return createWidget<Viewport>();
	}

	void Panel::destroyWidget(Ptr<Widget> widget)
	{
		auto it = getWidgetInfo(widget);
		widgetInfos.queueForErase(it);
	}

	void Panel::setWidgetBounds(Ptr<Widget> widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset)
	{
		auto & widgetInfo = *getWidgetInfo(widget);
		widgetInfo.originInPanel = originInPanel;
		widgetInfo.originInWidget = originInWidget;
		widgetInfo.originOffset = originOffset;
		widgetInfo.sizeInPanel = sizeInPanel;
		widgetInfo.sizeOffset = sizeOffset;
		updateWidgetBounds(widgetInfo);
	}

	void Panel::update(float dt)
	{
		for (auto const & widgetInfo : widgetInfos)
		{
			widgetInfo.widget->update(dt);
		}

		if (!widgetInfos.eraseQueueIsEmpty())
		{
			widgetInfos.processEraseQueue();
			float widgetDepth = depth;
			setDepth(widgetDepth);
		}
	}

	template <typename T> Ptr<T> Panel::createWidget()
	{
		auto widget = OwnPtr<T>::returnNew(getScene());
		WidgetInfo widgetInfo;
		widgetInfo.widget = widget;
		widgetInfos.push_back(widgetInfo);
		updateWidgetBounds(widgetInfo);
		float widgetDepth = depth;
		setDepth(widgetDepth);
		return widget;
	}

	std::list<Panel::WidgetInfo>::iterator Panel::getWidgetInfo(Ptr<Widget> widget)
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

	void Panel::updateWidgetBounds(WidgetInfo const & widgetInfo) const
	{
		Vector2f panelSize = Vector2f {bounds.max - bounds.min + Vector2i {1, 1}};
		Vector2f widgetSize = panelSize.scale(widgetInfo.sizeInPanel) + Vector2f {widgetInfo.sizeOffset};
		Vector2f widgetPosition = panelSize.scale(widgetInfo.originInPanel) - widgetSize.scale(widgetInfo.originInWidget) + Vector2f {widgetInfo.originOffset};
		widgetInfo.widget->setBounds(Recti {Vector2i{widgetPosition}, Vector2i{widgetPosition + widgetSize - Vector2f{1, 1}}});
	}
}