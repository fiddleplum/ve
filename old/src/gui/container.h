#pragma once

#include "sprite.h"
#include "viewport.h"
#include "../ptr.h"
#include <map>
#include <list>
#include <functional>

namespace ve
{
	namespace gui
	{
		// A base class of all elements which contain other elements.
		class Container : public Element
		{
		protected:
			void handleNewBounds() override;

			bool handleInputEvent(InputEvent const & event, std::optional<Vector2i> cursorPosition) override;

			void update(float dt) override;

			void preRenderUpdate() override;

			void render(Vector2i windowSize) const override;

			template <typename T> UsePtr<T> createElement();

			void destroyElement(UsePtr<Element> & element);

			void moveElementToFront(UsePtr<Element> const & element);

			void setElementActive(UsePtr<Element> const & element, bool active);

			void setElementPosition(UsePtr<Element> const & element, Vector2f fractionOfContainer, Vector2f fractionOfElement, Vector2i offset);

			void setElementSize(UsePtr<Element> const & element, Vector2f fractionOfContainer, Vector2i offset);

			void setContainerInputEventHandler(std::function<bool(InputEvent const & event, std::optional<Vector2i> cursorPosition)> handler);

			void setUpdateHandler(std::function<void(float dt)> handler);

			void setPreRenderUpdateHandler(std::function<void()> handler);

		private:
			class ElementInfo
			{
			public:
				OwnPtr<Element> element;
				bool active;
				Vector2f positionFractionOfElement;
				Vector2f positionFractionOfContainer;
				Vector2i positionOffset;
				Vector2f sizeFractionOfContainer;
				Vector2i sizeOffset;
			};

			void updateElementBounds(ElementInfo const & info);

			std::list<ElementInfo> infos;
			std::map<UsePtr<Element>, std::list<ElementInfo>::iterator> lookup;
			std::function<bool(InputEvent const & event, std::optional<Vector2i> cursorPosition)> inputEventHandler;
			std::function<void(float dt)> updateHandler;
			std::function<void()> preRenderUpdateHandler;
		};

		template <typename T> UsePtr<T> Container::addElement()
		{
			ElementInfo info;
			OwnPtr<T> elementDerived = OwnPtr<T>::createNew();
			info.element = elementDerived;
			info.active = true;
			info.sizeFractionOfContainer = { 1, 1 };
			info.positionFractionOfElement = { 0, 0 };
			info.positionFractionOfContainer = { 0, 0 };
			info.positionOffset = { 0, 0 };
			auto it = infos.insert(infos.end(), info);
			lookup[info.element] = it;
			updateElementBounds(info);
			return elementDerived;
		}
	}
}

