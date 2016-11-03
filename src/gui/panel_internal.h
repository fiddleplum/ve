#pragma once

#include "panel.h"
#include "widget_internal.h"
#include "object_vector.h"

namespace ve
{
	class PanelInternal : public virtual Panel, public virtual WidgetInternal
	{
	public:
		// Constructor.
		PanelInternal(UsePtr<Scene> scene);

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float depth) override;

		// Sets the bounds of the panel.
		void setBounds(Recti bounds) override;

		// Creates a sprite contained in the panel.
		UsePtr<Sprite> createSprite() override;

		// Creates a text button contained in the panel.
		UsePtr<TextButton> createTextButton() override;

		// Destroys a widget.
		void destroyWidget(UsePtr<Widget> widget) override;

		// Sets the relative bounds for the widget.
		void setBounds(UsePtr<Widget> widget, Coord2f originInPanel, Coord2f originInWidget, Coord2i originOffset, Coord2f sizeInPanel, Coord2i sizeOffset) override;

		// Updates the panel.
		void update(float dt) override;

	private:
		struct WidgetInfo
		{
			OwnPtr<WidgetInternal> widget;
			Coord2f originInPanel;
			Coord2f originInWidget;
			Coord2i originOffset;
			Coord2f sizeInPanel;
			Coord2i sizeOffset;
		};

		template <typename T> UsePtr<T> createWidget();
		std::vector<WidgetInfo>::iterator PanelInternal::getWidgetInfo(UsePtr<Widget> widget);
		void updateWidgetBounds(WidgetInfo const & widgetInfo) const;

		Recti bounds;
		ObjectVector<WidgetInfo> widgetInfos;
	};
}