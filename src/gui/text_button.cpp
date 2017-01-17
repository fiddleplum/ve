#include "text_button.hpp"

namespace ve
{
	TextButton::TextButton(Ptr<render::Scene> scene)
		: Widget(scene)
	{

	}

	float TextButton::getDepth() const
	{
		return 0;
	}

	void TextButton::setDepth(float & depth)
	{
		depth++;
	}

	Recti TextButton::getBounds() const
	{
		return Recti();
	}

	void TextButton::setBounds(Recti bounds_)
	{

	}

	void TextButton::onCursorPositionChanged(std::optional<Vector2i> cursorPosition)
	{

	}

	void TextButton::update(float dt)
	{

	}

	TextButton::~TextButton()
	{

	}
}