#include "CachedImage.h"

namespace MT
{
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
		serializationClient->setBool("shouldScaleToImageSize", flag);
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

		if (imageData == nullptr || serializationClient->getInt("imageDataSize") < w * h * 3)
		{
			// HACK: Manual memory is MUCH faster than std::unique_ptr
			imageData = new char[w * h * 3];
			serializationClient->setInt("imageDataSize", w * h * 3);
		}

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

			if (cachedTexture == nullptr)
			{
				cachedTexture = std::make_shared<MT::Texture>(modules->screen);
			}

			cachedTexture->rebindWithRawPixelData(imageData, w, h);
			setTexture(cachedTexture);

			if (serializationClient->getBool("shouldScaleToImageSize", true))
			{
				setSize(w, h);
			}
		}
		else
		{
			setSize(0, 0);
			MT::Logger::instance()->logCritical("CachedImage::OpenGL Error reported: " + std::to_string(err));
		}
	}
}
