#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include <SDL.h>
#include "engine/module/input/IInputListener.h"

namespace MT
{

	class Keyboard
	{
		std::unordered_map<SDL_Scancode, std::list<std::weak_ptr<IInputListener>>> onKeyListeners;
		std::unordered_map<SDL_Scancode, Uint8> oldKeyValues;
	public:

		void registerKey(SDL_Scancode code, std::weak_ptr<IInputListener> listener);
		void registerKeys(std::vector<SDL_Scancode> codes, std::weak_ptr<IInputListener> listener);
		void unregisterKey(SDL_Scancode code, std::weak_ptr<IInputListener> listener);
		void unregisterObject(std::weak_ptr<IInputListener> listener);

		void updateKeyStates(SDL_Event* event);

		void purgeWeakRefs();
	};

}
