#include "Mouse.h"

namespace AW
{
	void Mouse::handleMouseMove(int x, int y)
	{
		if (x != this->x || y != this->y)
		{
			for (auto listener : mouseMoveListeners)
			{
				auto ptr = (listener).ptr.lock();
				if (ptr != nullptr)
				{
					ptr->mouseMove(x, y);
				}
			}

			this->x = x;
			this->y = y;
		}
	}

	void Mouse::handleMouseButtons(bool leftDown, bool rightDown, bool middleDown)
	{
		isProcessing = true;
		if (this->leftDown != leftDown)
		{
			fireMouseButtonEvent(AWMouseButton::Left, leftDown);
			this->leftDown = leftDown;
		}

		if (this->rightDown != rightDown)
		{
			fireMouseButtonEvent(AWMouseButton::Right, rightDown);
			this->rightDown = rightDown;
		}

		if (this->middleDown != middleDown)
		{
			fireMouseButtonEvent(AWMouseButton::Middle, middleDown);
			this->middleDown = middleDown;
		}
		isProcessing = false;

		for (const auto remove : buttonRemoves)
		{
			unregisterMouseButton(remove.first, remove.second);
		}
		buttonRemoves.clear();


	}

	void Mouse::handleWheel(int wheelX, int wheelY)
	{
		isProcessing = true;
		for (auto listener : mouseWheelListeners)
		{
			auto ptr = (listener).ptr.lock();
			if (ptr != nullptr)
			{
				ptr->mouseWheel(wheelX, wheelY);
			}
		}
		isProcessing = false;

		for (const auto remove : wheelRemoves)
		{
			unregisterMouseWheel(remove);
		}
		wheelRemoves.clear();
	}

	void Mouse::fireMouseButtonEvent(AWMouseButton button, bool pressed)
	{
		for (auto& listenerVtr : mouseButtonListeners)
		{
			if (listenerVtr.first == button)
				for (auto listener : listenerVtr.second)
				{
					auto ptr = listener.ptr.lock();
					if (ptr != nullptr)
					{
						ptr->mouseButton(button, pressed);
					}
				}
		}
	}

	void Mouse::registerMouseButton(AWMouseButton button, std::weak_ptr<IInputListener> listener)
	{
		const auto ptr = listener.lock();
		if (ptr != nullptr)
		{
			mouseButtonListeners[button].insert(IInputListenerObjectBundle{ ptr->inputListenerObjectId(), listener });
		}
	}

	void Mouse::registerMouseMotion(std::weak_ptr<IInputListener> listener)
	{
		const auto ptr = listener.lock();
		if (ptr != nullptr)
		{
			mouseMoveListeners.insert(IInputListenerObjectBundle{ ptr->inputListenerObjectId(), listener });
		}
	}

	void Mouse::registerMouseWheel(std::weak_ptr<IInputListener> listener)
	{
		const auto ptr = listener.lock();
		if (ptr != nullptr)
		{
			mouseWheelListeners.insert(IInputListenerObjectBundle{ ptr->inputListenerObjectId(), listener });
		}
	}

	void Mouse::updateMouseState(SDL_Event* event)
	{
		int x, y;
		int mouseStates = SDL_GetMouseState(&x, &y);
		int leftDown = mouseStates & SDL_BUTTON(SDL_BUTTON_LEFT);
		int rightDown = mouseStates & SDL_BUTTON(SDL_BUTTON_RIGHT);
		int middleDown = mouseStates & SDL_BUTTON(SDL_BUTTON_MIDDLE);

		handleMouseMove(x, y);
		handleMouseButtons(leftDown, rightDown, middleDown);

		if (event->type == SDL_MOUSEWHEEL)
		{
			handleWheel(event->wheel.x, event->wheel.y);
		}
	}

	void Mouse::purgeWeakRefs()
	{
		for (auto it = mouseMoveListeners.begin(); it != mouseMoveListeners.end();)
		{
			const auto ptr = (*it).ptr.lock();
			if (ptr == nullptr)
			{
				it = mouseMoveListeners.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (auto it = mouseWheelListeners.begin(); it != mouseWheelListeners.end();)
		{
			const auto ptr = (*it).ptr.lock();
			if (ptr == nullptr)
			{
				it = mouseWheelListeners.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (auto& mouseButtonListener : mouseButtonListeners)
		{
			for (auto it = mouseButtonListener.second.begin(); it != mouseButtonListener.second.end();)
			{
				const auto ptr = (*it).ptr.lock();
				if (ptr == nullptr)
				{
					it = mouseButtonListener.second.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}

	void Mouse::unregisterMouseButton(AWMouseButton button, std::shared_ptr<IInputListener> obj)
	{
		if (isProcessing)
		{
			buttonRemoves.push_back(std::make_pair(button, obj));
		}
		else
		{
			for (auto it = mouseButtonListeners[button].begin(); it != mouseButtonListeners[button].end();)
			{
				const auto objPtr = (*it).ptr.lock();
				if (objPtr == nullptr || objPtr->inputListenerObjectId() == obj->inputListenerObjectId())
				{
					it = mouseButtonListeners[button].erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}

	void Mouse::unregisterMouseWheel(std::shared_ptr<IInputListener> obj)
	{
		if (isProcessing)
		{
			wheelRemoves.push_back(obj);
		}
		else
		{
			for (auto it = mouseWheelListeners.begin(); it != mouseWheelListeners.end();)
			{
				const auto objPtr = (*it).ptr.lock();
				if (objPtr == nullptr || objPtr->inputListenerObjectId() == obj->inputListenerObjectId())
				{
					it = mouseWheelListeners.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}

	int Mouse::X()
	{
		return x;
	}

	int Mouse::Y()
	{
		return y;
	}
}
