#pragma once

namespace AW
{
	class ILuaCallbackTarget
	{
	public:
		virtual int getLuaBindingId() = 0;
		virtual int onLuaCallback(const std::string& func) = 0;
	};
}
