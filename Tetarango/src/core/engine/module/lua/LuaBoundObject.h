#pragma once

#include "ILuaObject.h"

#include "lua.hpp"

namespace AW
{
	class LuaBoundObject
	{
	public:
		LuaBoundObject() {};

		LuaBoundObject(const std::string& fnName, void(*callback)(LuaBoundObject*), const std::shared_ptr<ILuaObject>& callbackObj, lua_CFunction lFn)
			: fnName(fnName), callback(callback), callbackObj(callbackObj), luaFunction(lFn)
		{
		}

		std::string fnName;
		lua_CFunction luaFunction;
		void (*callback)(LuaBoundObject*);
		std::weak_ptr<ILuaObject> callbackObj;

		unsigned int args = 0;
		std::vector<std::string> argV;
		std::vector<std::string> returnValues;
	};
}

