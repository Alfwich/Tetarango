#pragma once

#include "ILuaCallbackTarget.h"

#include "lua.hpp"

namespace AW
{
	class LuaBoundObject
	{
	public:
		LuaBoundObject() {};

		LuaBoundObject(const std::string& fnName, void(*callback)(LuaBoundObject*), std::shared_ptr<ILuaCallbackTarget> callbackObj, lua_CFunction lFn)
			: fnName(fnName), callback(callback), callbackObj(callbackObj), luaFunction(lFn)
		{
		}

		std::string fnName;
		lua_CFunction luaFunction;
		void (*callback)(LuaBoundObject*);
		std::weak_ptr<ILuaCallbackTarget> callbackObj;

		int nArgs = 0;
		std::vector<std::string> args;
		std::vector<std::string> returnValues;
	};


}

