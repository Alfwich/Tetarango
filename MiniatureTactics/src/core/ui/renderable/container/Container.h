#pragma once

#include "engine/ApplicationObject.h"
#include "ui/renderable/Renderable.h"

namespace MT
{

	class Container  : public ApplicationObject, public Renderable 
	{ 
	public:
		Container();

		void setExpandToChildren(bool flag);
		void setSizeToScreenSize();
		virtual void onDestroyChildren();
		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual void add(std::shared_ptr<ApplicationObject> obj);
	};

}
