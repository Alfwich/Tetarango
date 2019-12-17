#pragma once

#include "Element.h"

namespace AW
{

	class Rectangle : public Element
	{
	public:
		Rectangle();

		void onLoadResources();
		void onInitialAttach();
	};

}
