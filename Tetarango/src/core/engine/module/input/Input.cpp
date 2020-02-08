#include "Input.h"

namespace
{
	const auto registerKeyLuaFunctionName = "registerKey";
}

namespace AW
{
	Input::Input()
	{
		keyboard = std::make_unique<Keyboard>();
		mouse = std::make_unique<Mouse>();
		gamepad = std::make_unique<Gamepad>();
	}

	void Input::bindLua(std::shared_ptr<Lua> lua)
	{
		this->lua = lua;
	}

	void Input::onBindLuaHooks(const std::shared_ptr<Lua>& lua)
	{
		lua->registerBoundFunction(registerKeyLuaFunctionName, shared_from_this());
	}

	void Input::onCleanup()
	{
		gamepad->closeAllOpenGamepads();
	}

	void Input::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == registerKeyLuaFunctionName && obj->args.size() > 1)
		{
			std::vector<AWKey> keys;
			for (auto i = size_t{ 1 }, end = obj->args.size(); i < end; ++i)
			{
				keys.push_back((AWKey)(std::stoi(obj->args[i])));
			}
			keyboard->registerKeys(keys, std::dynamic_pointer_cast<IInputListener>(lua->getILuaObjectObjectForBindingId(obj->args[0])));
		}
	}
}
