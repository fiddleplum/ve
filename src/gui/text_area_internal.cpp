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

	void TextAreaInternal::setFont(std::string const & fontFace, int fontSize)
	{
		auto store = getAppInternal()->getStoreInternal();
		font = store->getFont(fontFace + std::to_string(fontSize));
		if (!font)
		{
			font = store->loadFont(fontFace + std::to_string(fontSize), fontFace, fontSize);
		}
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