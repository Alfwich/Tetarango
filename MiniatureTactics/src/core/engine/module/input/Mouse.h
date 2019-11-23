#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include "engine/module/input/IInputListener.h"

namespace MT
{

	class Mouse
	{
		std::vector<std::pair<MouseButton, std::shared_ptr<IInputListener>>> removes;
		std::list<std::weak_ptr<IInputListener>> mouseMoveListeners;
		std::unordered_map<MouseButton, std::list<std::weak_ptr<IInputListener>>> mouseButtonListeners;
		int x = 0, y = 0;
		bool leftDown = false, rightDown = false, middleDown = false, isProcessing = false;

		void handleMouseMove(int x, int y);
		void handleMouseButtons(bool leftDown, bool rightDown, bool middleDown);
		void fireMouseButtonEvent(MouseButton button, bool pressed);

	public:
		void registerMouseButton(MouseButton button, std::weak_ptr<IInputListener> listener);
		void registerMouseMotion(std::weak_ptr<IInputListener> listener);

		void updateMouseState(SDL_Event* event);
		void purgeWeakRefs();

		void unregisterMouseButton(MouseButton button, std::shared_ptr<IInputListener> obj);

		int X();
		int Y();
	};

}
