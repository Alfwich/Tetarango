#pragma once

#include "Element.h"

namespace MT
{

	class CachedImage : public Element
	{
		char* imageData = nullptr;
		std::shared_ptr<MT::Texture> cachedTexture;

	public:
		CachedImage();
		~CachedImage();

		void setShouldScaleToImageSize(bool flag);
		void captureScreen(int x, int y, int w, int h);
		void captureWholeScreen();
	};

}

