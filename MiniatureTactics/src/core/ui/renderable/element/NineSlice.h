#pragma once

#include "Element.h"

namespace MT
{

	class NineSlice : public Element
	{
	public:
		NineSlice();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
	};


}
