#pragma once

#include "Element.h"

namespace AW
{
	class Cached : public Element
	{
		std::shared_ptr<Texture> cachedTexture;
		std::shared_ptr<Color> clearColor;

	public:
		Cached();

		void onLoadResources();
		void onBindShaders();

		void setClearColor(int r, int g, int b, int a = 255);
		const std::shared_ptr<Color>& getClearColor();
	};
}
