#include "CachedImage.h"

namespace
{
	const auto qualityFactorParamName = "qualityFactor";
	const auto shouldSerializeImageParamName = "shouldSerializeImage";
	const auto shouldScaleToImageParamName = "shouldScaleToImageSize";
	const auto imageWidthParamName = "imageWidth";
	const auto imageHeightParamName = "imageHeight";
	const auto imageDataSizeParamName = "imageSize";
}

namespace MT
{
	void CachedImage::updateCachedImage(std::shared_ptr<MT::ImageBundle> bundle)
	{
		const auto texture = std::make_shared<MT::Texture>(modules->screen);
		texture->rebindWithImageBundle(bundle);
		setTexture(texture);

		if (serializationClient->getBool(shouldScaleToImageParamName, true))
		{
			setSize(bundle->width, bundle->height);
		}
	}

	std::shared_ptr<ImageBundle> CachedImage::compressImage()
	{
		const auto result = std::make_shared<MT::ImageBundle>();

		modules->asset->compressRawImageToPng(imageData, result);

		return result;
	}

	void CachedImage::updateImageDataBuffer(int size)
	{
		if (imageData == nullptr)
		{
			imageData = std::make_shared<MT::ImageBundle>();
		}

		if (imageData->data.capacity() < size)
		{
			imageData->data.resize(size);
			imageData->type = MT::ImageBundleType::Unspecificed;
		}
	}

	void CachedImage::updateImageDataBuffer(int w, int h)
	{
		updateImageDataBuffer(w * h * 3);
	}

	bool CachedImage::captureScreenData(int x, int y, int w, int h)
	{
		updateImageDataBuffer(w, h);

		auto dataPtr = &imageData->data[0];
		glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, dataPtr);

		GLenum err = glGetError();

		if (err == GL_NO_ERROR)
		{
			int imageDataRow = 0, dataRow = h - 1, imageDataCol = 0, dataCol = 0, imageDataOffset = 0, dataOffset = 0, endRow = h / 2;
			char r, g, b;
			while (imageDataRow < endRow)
			{
				imageDataCol = 0;
				dataCol = 0;
				imageDataOffset = (imageDataRow * w * 3);
				dataOffset = (dataRow * w * 3);
				while (imageDataCol < w)
				{
					r = *(dataPtr + (imageDataCol * 3) + 0 + imageDataOffset);
					g = *(dataPtr + (imageDataCol * 3) + 1 + imageDataOffset);
					b = *(dataPtr + (imageDataCol * 3) + 2 + imageDataOffset);

					*(dataPtr + (imageDataCol * 3) + 0 + imageDataOffset) = *(dataPtr + (dataCol * 3) + 0 + dataOffset);
					*(dataPtr + (imageDataCol * 3) + 1 + imageDataOffset) = *(dataPtr + (dataCol * 3) + 1 + dataOffset);
					*(dataPtr + (imageDataCol * 3) + 2 + imageDataOffset) = *(dataPtr + (dataCol * 3) + 2 + dataOffset);

					*(dataPtr + (dataCol * 3) + 0 + dataOffset) = r;
					*(dataPtr + (dataCol * 3) + 1 + dataOffset) = g;
					*(dataPtr + (dataCol * 3) + 2 + dataOffset) = b;

					imageDataCol++;
					dataCol++;
				}

				imageDataRow++;
				dataRow--;
			}

			imageData->type = MT::ImageBundleType::Raw;
			imageData->width = w;
			imageData->height = h;
			serializationClient->setInt(imageWidthParamName, w);
			serializationClient->setInt(imageHeightParamName, h);
			return true;
		}

		MT::Logger::instance()->logCritical("CachedImage::OpenGL Error reported: " + std::to_string(err));
		return false;
	}

	CachedImage::CachedImage()
	{
		enableSerialization<CachedImage>();
	}

	int CachedImage::cachedImageWidth()
	{
		return serializationClient->getInt(imageWidthParamName);
	}

	int CachedImage::cachedImageHeight()
	{
		return serializationClient->getInt(imageHeightParamName);
	}

	void CachedImage::setShouldScaleToImageSize(bool flag)
	{
		serializationClient->setBool(shouldScaleToImageParamName, flag);
	}

	void CachedImage::setShouldSerializeImage(bool flag)
	{
		serializationClient->setBool(shouldSerializeImageParamName, flag);
	}

	void CachedImage::onInitialAttach()
	{
		Element::onInitialAttach();

		if (serializationClient->getBool(shouldSerializeImageParamName))
		{
			const auto client = serializationClient->getClient("__cached_image__");
			const auto data = serializationClient->getString("image-data");
			const auto w = serializationClient->getInt(imageWidthParamName);
			const auto h = serializationClient->getInt(imageHeightParamName);
			const auto end = data.size();

			if (!data.empty() && w > 0 && h > 0 && end > 0)
			{
				updateImageDataBuffer((int)end);
				imageData->data.clear();
				std::copy(data.begin(), data.end(), std::back_inserter(imageData->data));
				imageData->type = MT::ImageBundleType::Png;
				imageData->width = w;
				imageData->height = h;
				updateCachedImage(imageData);
			}
		}
	}

	void CachedImage::captureWholeScreen()
	{
		captureScreen(0, 0, modules->screen->getWidth(), modules->screen->getHeight());
	}

	void CachedImage::captureScreen(int x, int y, int w, int h)
	{
		if (x < 0)
		{
			x = 0;
		}

		if (y < 0)
		{
			y = 0;
		}

		if (x + w > modules->screen->getWidth())
		{
			w -= x + w - modules->screen->getWidth();
		}

		if (y + h > modules->screen->getHeight())
		{
			h -= y + h - modules->screen->getHeight();
		}

		if (w <= 0 || h <= 0)
		{
			return;
		}

		if (captureScreenData(x, y, w, h))
		{
			updateCachedImage(imageData);
		}
	}

	std::shared_ptr<MT::SerializationClient> CachedImage::doSerialize(MT::SerializationHint hint)
	{
		if (imageData != nullptr && serializationClient->getBool(shouldSerializeImageParamName) && hint == MT::SerializationHint::SERIALIZE)
		{
			std::string data;
			const auto client = serializationClient->getClient("__cached_image__", hint);
			if (imageData->type == MT::ImageBundleType::Raw)
			{
				auto pngImageData = std::make_shared<MT::ImageBundle>();
				modules->asset->compressRawImageToPng(imageData, pngImageData);
				imageData = pngImageData;
			}

			if (imageData->type == MT::ImageBundleType::Png)
			{
				for (const auto c : imageData->data)
				{
					data += c;
				}

			}

			serializationClient->setString("image-data", data);
		}

		return Element::doSerialize(hint);
	}


}
