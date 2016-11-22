#pragma once

#include "gui/panel.h"
#include "gui/widget_internal.h"
#include "util/object_list.h"

namespace ve
{
	class PanelInternal : public virtual Panel, public virtual WidgetInternal
	{
	public:
		// Constructor.
		PanelInternal(Ptr<Scene> scene);

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Sets the bounds of the panel.
		void setBounds(Recti bounds) override;

		// Creates a sprite contained in the panel.
		Ptr<Sprite> createSprite() override;

		// Creates a text button contained in the panel.
		Ptr<TextButton> createTextButton() override;

		// Destroys a widget.
		void destroyWidget(Ptr<Widget> widget) override;

		// Sets the relative bounds for the widget.
		void setWidgetBounds(Ptr<Widget> widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset) override;

		// Updates the panel.
		void update(float dt) override;

	private:
		struct WidgetInfo
		{
			OwnPtr<WidgetInternal> widget;
			Vector2f originInPanel;
			Vector2f originInWidget;
			Vector2i originOffset;
			Vector2f sizeInPanel;
			Vector2i sizeOffset;
		};

		template <typename T> Ptr<T> createWidget();
		std::list<WidgetInfo>::iterator PanelInternal::getWidgetInfo(Ptr<Widget> widget);
		void updateWidgetBounds(WidgetInfo const & widgetInfo) const;

		Recti bounds;
		ObjectList<WidgetInfo> widgetInfos;
	};
}