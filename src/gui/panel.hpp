#pragma once

#include "util/ptr_list.hpp"
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
		Panel(Ptr<render::Scene> const & scene, Ptr<render::Shader> const & shader);

		// Internal to gui. Returns the depth.
		float getDepth() const override;

		// Internal to gui. Sets the depth.
		void setDepth(float depth) override;

		// Returns the bounds.
		Recti getBounds() const override;

		// Internal to gui. Sets the bounds of the panel.
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
		void destroyWidget(Ptr<Widget> const & widget);

		// Sets the relative bounds for the widget.
		void setWidgetBounds(Ptr<Widget> const & widget, Vector2f originInPanel, Vector2f originInWidget, Vector2i originOffset, Vector2f sizeInPanel, Vector2i sizeOffset);

		// Internal to gui. Called when the user moves the cursor within the widget or out of the widget.
		void onCursorPositionChanged(std::optional<Vector2i> cursorPosition) override;

		// Internal to gui. Updates the panel.
		void update(float dt) override;

	private:
		struct WidgetInfo
		{
			Vector2f originInPanel;
			Vector2f originInWidget;
			Vector2i originOffset;
			Vector2f sizeInPanel;
			Vector2i sizeOffset;
		};

		template <typename T> Ptr<T> createWidget();
		void updateWidgetBounds(Ptr<Widget> const & widget) const;

		Recti bounds;
		float depth;
		PtrList<Widget> widgets;
		std::map<Ptr<Widget>, WidgetInfo> widgetInfos;
	};
}