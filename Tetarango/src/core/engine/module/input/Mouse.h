#pragma once

#include "engine/module/input/IInputListener.h"

namespace AW
{

	class Mouse
	{
		std::vector<std::pair<AWMouseButton, std::shared_ptr<IInputListener>>> buttonRemoves;
		std::vector<std::shared_ptr<IInputListener>> wheelRemoves;

		std::unordered_set<IInputListenerObjectBundle, input_listener_bundle_object_hash> mouseMoveListeners;
		std::unordered_set<IInputListenerObjectBundle, input_listener_bundle_object_hash> mouseWheelListeners;
		std::unordered_map<AWMouseButton, std::unordered_set<IInputListenerObjectBundle, input_listener_bundle_object_hash>> mouseButtonListeners;
		int x = 0, y = 0, wX = 0, wY = 0;
		bool leftDown = false, rightDown = false, middleDown = false, isProcessing = false;

		void handleMouseMove(int x, int y);
		void handleMouseButtons(bool leftDown, bool rightDown, bool middleDown);
		void handleWheel(int wheelX, int wheelY);
		void fireMouseButtonEvent(AWMouseButton button, bool pressed);

	public:
		void registerMouseButton(AWMouseButton button, std::weak_ptr<IInputListener> listener);
		void registerMouseMotion(std::weak_ptr<IInputListener> listener);
		void registerMouseWheel(std::weak_ptr<IInputListener> listener);

		void updateMouseState(SDL_Event* event);
		void purgeWeakRefs();

		void unregisterMouseButton(AWMouseButton button, std::shared_ptr<IInputListener> obj);
		void unregisterMouseWheel(std::shared_ptr<IInputListener> obj);

		int X();
		int Y();
	};

}
