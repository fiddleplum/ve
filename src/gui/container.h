#pragma once

#include "element.h"
#include "../ptr.h"
#include <map>
#include <list>
#include <functional>

namespace ve
{
	namespace gui
	{
		class Container : public Element
		{
		public:
			Recti getBounds() const override;

			void setPosition(Vector2i position) override;

			void setSize(Vector2i size) override;

			template <typename T> UsePtr<T> addElement();

			void removeElement(UsePtr<Element> const & element);

			void moveElementToFront(UsePtr<Element> const & element);

			void setElementActive(UsePtr<Element> const & element, bool active);

			void setElementPosition(UsePtr<Element> const & element, Vector2f fractionOfContainer, Vector2f fractionOfElement, Vector2i offset);

			void setElementSize(UsePtr<Element> const & element, Vector2f fractionOfContainer, Vector2i offset);

			void setContainerEventHandler(std::function<bool(Event const & event, std::optional<Vector2i> cursorPosition)> handler);

			void setUpdateHandler(std::function<void(float dt)> handler);

			void setPreRenderUpdateHandler(std::function<void()> handler);

			bool handleEvent(Event const & event, std::optional<Vector2i> cursorPosition) override;

			void update(float dt) override;

			void preRenderUpdate() override;

			void render(Vector2i windowSize) const override;

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

			std::list<ElementInfo>::iterator find(UsePtr<Element> const & element) const;
			void updateElementBounds(ElementInfo const & info);

			Recti bounds;
			std::list<ElementInfo> infos;
			std::map<UsePtr<Element>, std::list<ElementInfo>::iterator> lookup;
			std::function<bool(Event const & event, std::optional<Vector2i> cursorPosition)> eventHandler;
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

