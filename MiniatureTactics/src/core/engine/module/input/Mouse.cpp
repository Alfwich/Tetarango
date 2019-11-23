#include "Mouse.h"

namespace MT
{

	void Mouse::handleMouseMove(int x, int y)
	{
		if (x != this->x || y != this->y)
		{
			std::list<std::weak_ptr<IInputListener>> removes;
			for (auto listener : mouseMoveListeners)
			{
				auto ptr = (listener).lock();
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
			fireMouseButtonEvent(MouseButton::Left, leftDown);
			this->leftDown = leftDown;
		}

		if (this->rightDown != rightDown)
		{
			fireMouseButtonEvent(MouseButton::Right, rightDown);
			this->rightDown = rightDown;
		}

		if (this->middleDown != middleDown)
		{
			fireMouseButtonEvent(MouseButton::Middle, middleDown);
			this->middleDown = middleDown;
		}
		isProcessing = false;

		for (const auto remove : removes)
		{
			unregisterMouseButton(remove.first, remove.second);
		}
		removes.clear();
	}

	void Mouse::fireMouseButtonEvent(MouseButton button, bool pressed)
	{
		for (auto& listenerVtr : mouseButtonListeners)
		{
			if (listenerVtr.first == button)
				for (auto listener : listenerVtr.second)
				{
					auto ptr = listener.lock();
					if (ptr != nullptr)
					{
						ptr->mouseButton(button, pressed);
					}
				}
		}
	}

	void Mouse::registerMouseButton(MouseButton button, std::weak_ptr<IInputListener> listener)
	{
		mouseButtonListeners[button].push_back(listener);
	}

	void Mouse::registerMouseMotion(std::weak_ptr<IInputListener> listener)
	{
		mouseMoveListeners.push_back(listener);
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
	}

	void Mouse::purgeWeakRefs()
	{
		for (auto it = mouseMoveListeners.begin(); it != mouseMoveListeners.end();)
		{
			const auto ptr = (*it).lock();
			if (ptr == nullptr)
			{
				it = mouseMoveListeners.erase(it);
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
				const auto ptr = (*it).lock();
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

	void Mouse::unregisterMouseButton(MouseButton button, std::shared_ptr<IInputListener> obj)
	{
		if (isProcessing)
		{
			removes.push_back(std::make_pair(button, obj));
		}
		else
		{
			for (auto it = mouseButtonListeners[button].begin(); it != mouseButtonListeners[button].end();)
			{
				const auto objPtr = (*it).lock();
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

	int Mouse::X()
	{
		return x;
	}

	int Mouse::Y()
	{
		return y;
	}
}
