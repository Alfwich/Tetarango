#pragma once

#include "engine/module/input/IInputListener.h"

namespace AW
{
	class Keyboard
	{
		std::unordered_map<AWKey, std::unordered_set<IInputListenerObjectBundle, input_listener_bundle_object_hash>> onKeyListeners;
		std::unordered_map<AWKey, Uint8> oldKeyValues;
	public:

		void registerKey(AWKey code, std::weak_ptr<IInputListener> listener);
		void registerKeys(const std::vector<AWKey>& codes, std::weak_ptr<IInputListener> listener);
		void unregisterKey(AWKey code, std::weak_ptr<IInputListener> listener);
		void unregisterObject(std::weak_ptr<IInputListener> listener);

		void updateKeyStates(SDL_Event* event);

		void purgeWeakRefs();
	};

}
