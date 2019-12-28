#pragma once

#include "Element.h"

namespace AW
{
	class DisplayBuffer : public Element
	{
		std::shared_ptr<Texture> cachedTexture;
		std::shared_ptr<Color> clearColor;

	public:
		DisplayBuffer();

		void onLoadResources();
		void onBindShaders();

		void setClearColor(int r, int g, int b, int a = 255);
		void setClearColor(const Color& c);
		const std::shared_ptr<Color>& getClearColor();

		void rebuildInternalTexture();
	};
}
