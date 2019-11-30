#pragma once

#include "Element.h"

namespace MT
{

	class CachedImage : public Element
	{
		char* imageData = nullptr;
		std::shared_ptr<MT::Texture> cachedTexture;

		void compressImage();
		void updateImageDataBuffer(int w, int h);
		bool captureScreenData(int x, int y, int w, int h);
		void updateCachedImageData(char* data, int w, int h);

	public:
		CachedImage();
		~CachedImage();

		void setShouldScaleToImageSize(bool flag);
		void setShouldSerializeImage(bool flag);
		void captureScreen(int x, int y, int w, int h);
		void captureWholeScreen();

		void onInitialAttach();

		std::shared_ptr<MT::SerializationClient> doSerialize(MT::SerializationHint hint);
	};

}

