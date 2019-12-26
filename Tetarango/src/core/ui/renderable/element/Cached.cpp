#include "Cached.h"

namespace AW
{
	Cached::Cached()
	{
		renderMode = RenderMode::CachedElement;
		renderUpdateMode = RenderUpdateMode::WhenDirty;
		registerGameObject<Cached>();
	}

	void Cached::setClearColor(int r, int g, int b, int a)
	{
		if (clearColor == nullptr)
		{
			clearColor = std::make_shared <Color>(r, g, b, a);
		}
		else
		{
			clearColor->r = r;
			clearColor->g = g;
			clearColor->b = b;
			clearColor->a = a;
		}
	}

	const std::shared_ptr<Color>& Cached::getClearColor()
	{
		return clearColor;
	}
}