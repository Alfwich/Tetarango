#pragma once

#include "Element.h"
#include "engine/module/asset/ImageBundle.h"

namespace AW
{

	class CachedImage : public Element
	{
		std::string textureBindingKey;
		std::shared_ptr<AW::ImageBundle> imageData;

		std::shared_ptr<AW::ImageBundle> compressImage();
		void updateImageDataBuffer(int size);
		void updateImageDataBuffer(int w, int h);
		bool captureScreenData(int x, int y, int w, int h);
		void updateCachedImage(std::shared_ptr<AW::ImageBundle> bundle);

	public:
		CachedImage();
		~CachedImage();

		int cachedImageWidth();
		int cachedImageHeight();
		void setShouldScaleToImageSize(bool flag);
		void setShouldSerializeImage(bool flag);
		void captureScreen(int x, int y, int w, int h);
		void captureWholeScreen();

		void onInitialAttach();
		std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);
	};

}

