#pragma once

#include "engine/ApplicationObject.h"
#include "ui/renderable/Renderable.h"
#include "ui/renderable/element/Rectangle.h"

namespace AW
{

	class Container : public ApplicationObject, public Renderable
	{
		bool isAutoLayingOut = false, shouldAutoLayout = true;

	public:
		Container();

		void setSizeToScreenSize();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

		void resizeSelfToChildrenAndCenterChildren();
		void performAutoLayoutIfNeeded();

		void setWidth(double newWidth);
		void setHeight(double newHeight);
	};

}
