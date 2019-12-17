#include "Rectangle.h"

namespace AW
{
	Rectangle::Rectangle()
	{
		registerSerialization<Rectangle>();
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
