#include "container.h"
#include "../render/open_gl.h"

namespace ve
{
	namespace gui
	{
		void Container::handleNewBounds()
		{
			for (auto const & info : infos)
			{
				updateElementBounds(info);
			}
		}

		bool Container::handleInputEvent(InputEvent const & event, std::optional<Vector2i> cursorPosition)
		{
			bool consumed = false;
			if (inputEventHandler)
			{
				consumed = inputEventHandler(event, cursorPosition);
			}
			bool cursorPositionIsValidInContainer = cursorPosition && bounds.contains(cursorPosition.value());
			for (auto it = infos.rbegin(); !consumed && it != infos.rend(); it++)
			{
				ElementInfo & info = *it;
				if (info.active)
				{
					consumed = info.element->handleInputEvent(event, cursorPosition);
				}
			}
			return consumed;
		}

		void Container::update(float dt)
		{
			if (updateHandler)
			{
				updateHandler(dt);
			}
			for (auto const & info : infos)
			{
				if (info.active)
				{
					info.element->update(dt);
				}
			}
		}

		void Container::preRenderUpdate()
		{
			if (preRenderUpdateHandler)
			{
				preRenderUpdateHandler();
			}
			for (auto const & info : infos)
			{
				if (info.active)
				{
					info.element->preRenderUpdate();
				}
			}
		}

		void Container::render(Vector2i windowSize) const
		{
			ve::glScissorPush(bounds.min[0], bounds.min[1], bounds.max[0] - bounds.min[0] + 1, bounds.max[1] - bounds.min[1] + 1);
			for (auto const & info : infos)
			{
				if (info.active)
				{
					info.element->render(windowSize);
				}
			}
			ve::glScissorPop();
		}

		void Container::destroyElement(UsePtr<Element> & element)
		{
			std::findelement)
			infos.erase();
			lookup.erase(element);
		}

		void Container::moveElementToFront(UsePtr<Element> const & element)
		{
			auto itOld = find(element);
			ElementInfo info = *itOld; // make a copy so it doesn't destruct
			infos.erase(itOld);
			lookup.erase(element);
			auto itNew = infos.insert(infos.end(), info);
			lookup[info.element] = itNew;
		}

		void Container::setElementActive(UsePtr<Element> const & element, bool active)
		{
			auto it = find(element);
			it->active = active;
		}

		void Container::setElementPosition(UsePtr<Element> const & element, Vector2f fractionOfContainer, Vector2f fractionOfElement, Vector2i offset)
		{
			auto it = find(element);
			it->positionFractionOfElement = fractionOfElement;
			it->positionFractionOfContainer = fractionOfContainer;
			it->positionOffset = offset;
			updateElementBounds(*it);
		}

		void Container::setElementSize(UsePtr<Element> const & element, Vector2f fractionOfContainer, Vector2i offset)
		{
			auto it = find(element);
			it->sizeFractionOfContainer = fractionOfContainer;
			it->sizeOffset = offset;
			updateElementBounds(*it);
		}

		void Container::setContainerInputEventHandler(std::function<bool(InputEvent const & event, std::optional<Vector2i> cursorPosition)> handler)
		{
			inputEventHandler = handler;
		}

		void Container::setUpdateHandler(std::function<void(float dt)> handler)
		{
			updateHandler = handler;
		}

		void Container::setPreRenderUpdateHandler(std::function<void()> handler)
		{
			preRenderUpdateHandler = handler;
		}

		std::list<Container::ElementInfo>::iterator Container::find(UsePtr<Element> const & element) const
		{
			auto it = lookup.find(element);
			if (it != lookup.end())
			{
				return it->second;
			}
			else
			{
				throw std::exception();
			}
		}

		void Container::updateElementBounds(ElementInfo const & info)
		{
			Vector2i containerSize = bounds.max - bounds.min + Vector2i{ 1, 1 };
			Recti elementBounds = info.element->getBounds();
			Vector2i elementSize = elementBounds.getSize();
			elementBounds.setSize(info.sizeOffset + (Vector2i)(info.sizeFractionOfContainer.scale((Vector2f)containerSize)));
			elementBounds.setPosition(info.positionOffset + (Vector2i)(info.positionFractionOfContainer.scale((Vector2f)containerSize)) - (Vector2i)(info.positionFractionOfElement.scale((Vector2f)elementBounds.getSize())));
		}
	}
}

