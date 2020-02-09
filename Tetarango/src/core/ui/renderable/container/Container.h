#pragma once

#include "engine/GameObject.h"
#include "ui/renderable/Renderable.h"
#include "ui/renderable/primitive/Rectangle.h"

namespace AW
{

	class Container : public GameObject, public Renderable
	{
		bool isAutoLayingOut = false, shouldAutoLayout = true;

	public:
		Container();

		void setSizeToScreenSize();

		virtual void onBindLuaHooks() override;

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

		void resizeSelfToChildrenAndCenterChildren();
		void performAutoLayoutIfNeeded();

		void setScreenWidth(double newWidth);
		void setScreenHeight(double newHeight);

		void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
	};

}
