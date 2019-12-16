#include "Rectangle.h"

namespace AWCore
{
	Rectangle::Rectangle()
	{
		enableSerialization<Rectangle>();
	}

	void Rectangle::onLoadResources()
	{
		modules->texture->loadTexture("res/core/img/solid.png", "__solid__");
	}

	void Rectangle::onInitialAttach()
	{
		setTexture("__solid__");
	}
}
