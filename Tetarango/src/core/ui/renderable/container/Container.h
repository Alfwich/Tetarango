#pragma once

#include "engine/ApplicationObject.h"
#include "ui/renderable/Renderable.h"

namespace MT
{

	class Container : public ApplicationObject, public Renderable 
	{ 
		bool isAutoLayingOut = false;
		void performAutoLayout();

	public:
		Container();

		void setExpandToChildren(bool flag);
		bool getExpandToChildren();
		void setSizeToScreenSize();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual void add(std::shared_ptr<ApplicationObject> obj);

		void setWidth(double newWidth);
		void setHeight(double newHeight);

		void doUpdateDebugChildren();
	};

}
