#pragma once

#include "Element.h"

namespace AWCore
{

	class Rectangle : public Element
	{
	public:
		Rectangle();

		void onLoadResources();
		void onInitialAttach();
	};

}
