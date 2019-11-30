#pragma once

#include "Element.h"
#include "engine/module/asset/ImageBundle.h"

namespace MT
{

	class CachedImage : public Element
	{
		std::shared_ptr<MT::ImageBundle> imageData;

		std::shared_ptr<MT::ImageBundle> compressImage();
		void updateImageDataBuffer(int size);
		void updateImageDataBuffer(int w, int h);
		bool captureScreenData(int x, int y, int w, int h);
		void updateCachedImage(std::shared_ptr<MT::ImageBundle> bundle);

	public:
		CachedImage();

		int cachedImageWidth();
		int cachedImageHeight();
		void setShouldScaleToImageSize(bool flag);
		void setShouldSerializeImage(bool flag);
		void captureScreen(int x, int y, int w, int h);
		void captureWholeScreen();

		void onInitialAttach();
		std::shared_ptr<MT::SerializationClient> doSerialize(MT::SerializationHint hint);
	};

}

