#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad/Gamepad.h"
#include "engine/module/IBaseModule.h"

namespace AW
{
	class Input : public IBaseModule
	{
	public:
		Input();

		void bindLua(std::shared_ptr<Lua> lua);
		std::shared_ptr<Lua> lua;

		std::shared_ptr<Keyboard> keyboard;
		std::shared_ptr<Mouse> mouse;
		std::shared_ptr<Gamepad> gamepad;

		virtual void onBindLuaHooks(const std::shared_ptr<Lua>& lua) override;
		virtual void onCleanup() override;

		virtual std::string getLuaBindingId() override { return "input"; };
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
	};
}
