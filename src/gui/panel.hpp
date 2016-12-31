#pragma once

#include "util/object_list.hpp"
#include "gui/widget.hpp"
#include "gui/sprite.hpp"
#include "gui/text_area.hpp"
#include "gui/text_button.hpp"
#include "gui/viewport.hpp"

namespace ve
{
	class Panel : public Widget
	{
	public:
		// Constructor.
		Panel(Ptr<render::Scene> scene);

		// Returns the depth.
		float getDepth() const override;

		// Sets the depth.
		void setDepth(float & depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Sets the bounds of the panel.
		void setBounds(Recti bounds) override;

		// Creates a sprite contained in the panel.
		Ptr<Sprite> createSprite();

		// Creates a text area contained in the panel.
		Ptr<TextArea> createTextArea();

		// Creates a text button contained in the panel.
		Ptr<TextButton> createTextButton();

		// Creates a viewport contained in the panel.
		Ptr<Viewport> createViewport();

		// Destroys a widget.
		void destroyWidget(Ptr<Widget> widget);

		// Sets the relative bounds for the widget.
		void setWidgetBounds(Ptr<Widget> widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset);

		// Updates the panel.
		void update(float dt) override;

	private:
		struct WidgetInfo
		{
			OwnPtr<Widget> widget;
			Vector2f originInPanel;
			Vector2f originInWidget;
			Vector2i originOffset;
			Vector2f sizeInPanel;
			Vector2i sizeOffset;
		};

		template <typename T> Ptr<T> createWidget();
		std::list<WidgetInfo>::iterator Panel::getWidgetInfo(Ptr<Widget> widget);
		void updateWidgetBounds(WidgetInfo const & widgetInfo) const;

		Recti bounds;
		float depth;
		ObjectList<WidgetInfo> widgetInfos;
	};
}