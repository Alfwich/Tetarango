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
	void CachedImage::updateCachedImageData(char* data, int w, int h)
	{
		if (cachedTexture == nullptr)
		{
			cachedTexture = std::make_shared<MT::Texture>(modules->screen);
		}

		cachedTexture->rebindWithRawPixelData(data, w, h);
		setTexture(cachedTexture);

		if (serializationClient->getBool(shouldScaleToImageParamName, true))
		{
			setSize(w, h);
		}
	}

	void CachedImage::updateImageDataBuffer(int w, int h)
	{
		if (imageData == nullptr || serializationClient->getInt(imageDataSizeParamName) < w * h * 3)
		{
			if (imageData != nullptr)
			{
				delete[] imageData;
			}

			// HACK: Manual memory is MUCH faster than std::unique_ptr
			imageData = new char[w * h * 3];
			serializationClient->setInt(imageDataSizeParamName, w * h * 3);
		}
	}

	bool CachedImage::captureScreenData(int x, int y, int w, int h)
	{
		updateImageDataBuffer(w, h);

		serializationClient->setInt(imageWidthParamName, w);
		serializationClient->setInt(imageHeightParamName, h);

		glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, imageData);

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
					r = *(imageData + (imageDataCol * 3) + 0 + imageDataOffset);
					g = *(imageData + (imageDataCol * 3) + 1 + imageDataOffset);
					b = *(imageData + (imageDataCol * 3) + 2 + imageDataOffset);

					*(imageData + (imageDataCol * 3) + 0 + imageDataOffset) = *(imageData + (dataCol * 3) + 0 + dataOffset);
					*(imageData + (imageDataCol * 3) + 1 + imageDataOffset) = *(imageData + (dataCol * 3) + 1 + dataOffset);
					*(imageData + (imageDataCol * 3) + 2 + imageDataOffset) = *(imageData + (dataCol * 3) + 2 + dataOffset);

					*(imageData + (dataCol * 3) + 0 + dataOffset) = r;
					*(imageData + (dataCol * 3) + 1 + dataOffset) = g;
					*(imageData + (dataCol * 3) + 2 + dataOffset) = b;

					imageDataCol++;
					dataCol++;
				}

				imageDataRow++;
				dataRow--;
			}

			return true;
		}

		MT::Logger::instance()->logCritical("CachedImage::OpenGL Error reported: " + std::to_string(err));
		return false;
	}

	CachedImage::CachedImage()
	{
		enableSerialization<CachedImage>();
	}

	CachedImage::~CachedImage()
	{
		if (imageData != nullptr)
		{
			delete[] imageData;
		}
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
			const auto end = serializationClient->getInt(imageDataSizeParamName);

			if (!data.empty() && w > 0 && h > 0 && end > 0)
			{
				updateImageDataBuffer(w, h);

				for (int i = 0; i < end; ++i)
				{
					imageData[i] = data[i];
				}

				updateCachedImageData(imageData, w, h);
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
			updateCachedImageData(imageData, w, h);
		}
	}

	std::shared_ptr<MT::SerializationClient> CachedImage::doSerialize(MT::SerializationHint hint)
	{
		if (serializationClient->getBool(shouldSerializeImageParamName) && hint == MT::SerializationHint::SERIALIZE)
		{
			const auto client = serializationClient->getClient("__cached_image__", hint);
			std::string data;
			for (int i = 0, end = serializationClient->getInt(imageDataSizeParamName); i < end; ++i)
			{
				data += imageData[i];
			}
			serializationClient->setString("image-data", data);
		}

		return Element::doSerialize(hint);
	}


}
