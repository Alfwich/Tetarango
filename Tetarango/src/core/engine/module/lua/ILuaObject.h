#pragma once


namespace AW
{
	class LuaBoundObject;

	class ILuaObject
	{
	public:
		virtual std::string getLuaBindingId() = 0;
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) = 0;
	};
}

#include "LuaBoundObject.h"
