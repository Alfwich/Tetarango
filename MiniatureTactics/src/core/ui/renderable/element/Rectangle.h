#pragma once

#include "Element.h"

namespace MT
{

	class Rectangle : public Element
	{
	public:
		Rectangle();

		void onLoadResources();
		void onInitialAttach();
	};

}
