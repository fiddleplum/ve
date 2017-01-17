#include "gui/panel.hpp"
#include <algorithm>

namespace ve
{
	Panel::Panel(Ptr<render::Scene> scene)
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
		for (auto const && widget : widgets)
		{
			widget->setDepth(depth_);
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
		for (auto const && widget : widgets)
		{
			updateWidgetBounds(widget);
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

	void Panel::destroyWidget(Ptr<Widget> const & widget)
	{
		for (auto iter = widgets.begin(); iter != widgets.end(); iter++)
		{
			if (*iter == widget)
			{
				widgets.queueForErase(iter);
				widgetInfos.erase(*iter);
				break;
			}
		}
	}

	void Panel::setWidgetBounds(Ptr<Widget> const & widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset)
	{
		auto & widgetInfo = widgetInfos[widget];
		widgetInfo.originInPanel = originInPanel;
		widgetInfo.originInWidget = originInWidget;
		widgetInfo.originOffset = originOffset;
		widgetInfo.sizeInPanel = sizeInPanel;
		widgetInfo.sizeOffset = sizeOffset;
		updateWidgetBounds(widget);
	}

	void Panel::onCursorPositionChanged(std::optional<Vector2i> cursorPosition)
	{
		for (auto const && widget : widgets)
		{
			auto widgetCursorPosition = cursorPosition;
			if (widgetCursorPosition && !widget->getBounds().contains(*widgetCursorPosition))
			{
				widgetCursorPosition = std::nullopt;
			}
			widget->onCursorPositionChanged(widgetCursorPosition);
		}
		widgets.processEraseQueue();
	}

	void Panel::update(float dt)
	{
		for (auto const && widget : widgets)
		{
			widget->update(dt);
		}
		widgets.processEraseQueue();
	}

	template <typename T> Ptr<T> Panel::createWidget()
	{
		Ptr<T> widget = widgets.appendNew<T>(getScene());
		widget->setDepth(depth);
		widgetInfos.insert({widget, WidgetInfo()});
		updateWidgetBounds(widget);
		return widget;
	}

	void Panel::updateWidgetBounds(Ptr<Widget> const & widget) const
	{
		auto & widgetInfo = widgetInfos.at(widget);
		Vector2f panelSize = Vector2f {bounds.max - bounds.min + Vector2i {1, 1}};
		Vector2f widgetSize = panelSize.scale(widgetInfo.sizeInPanel) + Vector2f {widgetInfo.sizeOffset};
		Vector2f widgetPosition = panelSize.scale(widgetInfo.originInPanel) - widgetSize.scale(widgetInfo.originInWidget) + Vector2f {widgetInfo.originOffset};
		widget->setBounds(Recti {Vector2i{widgetPosition}, Vector2i{widgetPosition + widgetSize - Vector2f{1, 1}}});
	}
}