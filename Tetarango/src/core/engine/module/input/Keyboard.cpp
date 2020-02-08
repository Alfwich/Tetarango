#include "Keyboard.h"

namespace AW
{
	void Keyboard::registerKey(AWKey code, std::weak_ptr<IInputListener> listener)
	{
		onKeyListeners[code].push_back(listener);
		oldKeyValues[code] = 0;
	}

	void Keyboard::registerKeys(const std::vector<AWKey>& codes, std::weak_ptr<IInputListener> listener)
	{
		for (auto code : codes)
		{
			registerKey(code, listener);
		}
	}

	void Keyboard::unregisterKey(AWKey code, std::weak_ptr<IInputListener> listener)
	{
		const auto objectToRemovePtr = listener.lock();
		for (auto it = onKeyListeners[code].begin(); it != onKeyListeners[code].end();)
		{
			if (const auto ptr = (*it).lock())
			{
				if (ptr == objectToRemovePtr)
				{
					onKeyListeners[code].erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
			else
			{
				it = onKeyListeners[code].erase(it);
			}
		}
	}

	void Keyboard::unregisterObject(std::weak_ptr<IInputListener> listener)
	{
		for (auto keyValuePair : oldKeyValues)
		{
			unregisterKey(keyValuePair.first, listener);
		}
	}

	void Keyboard::updateKeyStates(SDL_Event* event)
	{
		const Uint8* keyStates = SDL_GetKeyboardState(NULL);
		for (auto listenerKeyPair : onKeyListeners)
		{
			if (oldKeyValues[listenerKeyPair.first] != keyStates[(SDL_Scancode)listenerKeyPair.first])
			{
				for (auto listener : listenerKeyPair.second)
				{
					const auto ptr = listener.lock();
					if (ptr != nullptr)
					{
						ptr->key(listenerKeyPair.first, keyStates[(SDL_Scancode)listenerKeyPair.first]);
					}
				}
			}
		}

		for (auto keyValuePair : oldKeyValues)
		{
			oldKeyValues[keyValuePair.first] = keyStates[(SDL_Scancode)keyValuePair.first];
		}
	}

	void Keyboard::purgeWeakRefs()
	{
		for (auto& listenerKeyPair : onKeyListeners)
		{
			for (auto it = listenerKeyPair.second.begin(); it != listenerKeyPair.second.end();)
			{
				const auto ptr = (*it).lock();
				if (ptr == nullptr)
				{
					it = listenerKeyPair.second.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}


}
