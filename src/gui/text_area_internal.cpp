#include "text_area_internal.h"
#include "app_internal.h"

namespace ve
{
	TextAreaInternal::TextAreaInternal(Ptr<Scene> scene)
		: WidgetInternal(scene)
	{

	}

	TextAreaInternal::~TextAreaInternal()
	{

	}

	void TextAreaInternal::setFont(std::string const & fontName)
	{
		auto store = getAppInternal()->getStoreInternal();
		auto font = store->getFont(fontName);
		if (!font)
		{
			font = store->loadFont(fontName);
		}
	}

	void TextAreaInternal::setFontSize(int fontSize)
	{

	}

	void TextAreaInternal::setText(std::string const & text)
	{

	}

	void TextAreaInternal::setColor(Vector4f color)
	{

	}

	float TextAreaInternal::getDepth() const
	{
		return model->getDepth();
	}

	void TextAreaInternal::setDepth(float depth)
	{
		model->setDepth(depth);
	}

	Recti TextAreaInternal::getBounds() const
	{
		return bounds;
	}

	void TextAreaInternal::setBounds(Recti bounds_)
	{
		bounds = bounds;
	}

	void TextAreaInternal::update(float dt)
	{

	}
}