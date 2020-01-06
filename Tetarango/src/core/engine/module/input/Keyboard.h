#pragma once

#include "engine/module/input/IInputListener.h"

namespace AW
{
	class Keyboard
	{
		std::unordered_map<AWKey, std::list<std::weak_ptr<IInputListener>>> onKeyListeners;
		std::unordered_map<AWKey, Uint8> oldKeyValues;
	public:

		void registerKey(AWKey code, std::weak_ptr<IInputListener> listener);
		void registerKeys(std::vector<AWKey> codes, std::weak_ptr<IInputListener> listener);
		void unregisterKey(AWKey code, std::weak_ptr<IInputListener> listener);
		void unregisterObject(std::weak_ptr<IInputListener> listener);

		void updateKeyStates(SDL_Event* event);

		void purgeWeakRefs();
	};

}
