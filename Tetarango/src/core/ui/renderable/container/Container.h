#pragma once

#include "engine/GameObject.h"
#include "ui/renderable/Renderable.h"
#include "ui/renderable/element/Rectangle.h"

namespace AW
{

	class Container : public GameObject, public Renderable
	{
		bool isAutoLayingOut = false, shouldAutoLayout = true;

	public:
		Container();

		void onBindShaders();

		void setSizeToScreenSize();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

		void resizeSelfToChildrenAndCenterChildren();
		void performAutoLayoutIfNeeded();

		void setWidth(double newWidth);
		void setHeight(double newHeight);
	};

}
