#pragma once

#include "ILuaObject.h"

#include "lua.hpp"

namespace AW
{
	class LuaBoundObject
	{
		static int nextBundleId;
	public:
		LuaBoundObject() : bundleId(nextBundleId++) {};

		LuaBoundObject(const std::string& fnName, void(*callback)(LuaBoundObject*), const std::shared_ptr<ILuaObject>& callbackObj, lua_CFunction lFn)
			: bundleId(nextBundleId++), fnName(fnName), callback(callback), callbackObj(callbackObj), luaFunction(lFn)
		{
		}

		const int bundleId;
		std::string fnName;
		lua_CFunction luaFunction;
		void (*callback)(LuaBoundObject*);
		std::weak_ptr<ILuaObject> callbackObj;

		std::vector<std::string> args;
		std::vector<std::string> returnValues;
	};
}

