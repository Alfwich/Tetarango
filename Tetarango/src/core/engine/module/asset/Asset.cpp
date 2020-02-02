#include "Asset.h"

#include <png.h>

namespace
{
	const auto gobalAssetPackName = "asset.pack";
}

namespace AW
{
	void Asset::decodeAssetPack(std::string path)
	{
		SDL_RWops* file = SDL_RWFromFile(path.c_str(), "rb");
		if (file == nullptr)
		{
			return;
		}

		Sint64 res_size = SDL_RWsize(file);
		auto res = std::make_unique<char[]>(res_size);
		Sint64 nb_read_total = 0, nb_read = 1, pos = 0;

		while (nb_read_total < res_size && nb_read != 0)
		{
			nb_read = SDL_RWread(file, res.get() + pos, 1, (res_size - nb_read_total));
			nb_read_total += nb_read;
			pos += nb_read;
		}

		SDL_RWclose(file);

		unsigned int i = 0, p = 0;
		while (i < res_size)
		{
			res[i] = res[i] ^ key[p++ % key.size()];
			++i;
		}

		cursorPosition = 0;

		unsigned int kPos = 0;
		while (cursorPosition < res_size)
		{
			if (!checkAndMoveIfCorrect(res.get(), assetStart))
			{
				Logger::instance()->logFatal("Asset::Failed to read asset package.");
			}

			std::string assetName = getAssetNameFromRaw(res.get());

			if (!checkAndMoveIfCorrect(res.get(), binaryDelimiter))
			{
				Logger::instance()->logFatal("Asset::Failed to read asset package.");
			}

			unsigned int size;
			std::unique_ptr<char[]> data = getAssetDataFromRaw(res.get(), size, kPos);

			if (!checkAndMoveIfCorrect(res.get(), assetEnd))
			{
				Logger::instance()->logFatal("Asset::Failed to read asset package.");
			}

			assetPack[assetName] = std::make_shared<ResourceBundle>(std::move(data), size);
		}
	}

	bool Asset::checkAndMoveIfCorrect(char* data, std::string tag)
	{
		if (beginsWith(data, cursorPosition, tag))
		{
			cursorPosition += (unsigned int)tag.size();
			return true;
		}

		return false;
	}

	bool Asset::beginsWith(char* data, unsigned int pos, std::string tag)
	{
		const auto tagSize = tag.size();

		for (unsigned int i = 0; i < tagSize; ++i)
		{
			if (*(data + i + pos) != tag[i])
			{
				return false;
			}
		}

		return true;
	}

	std::string Asset::assetNameFromPath(std::string path)
	{
		std::string result;
		unsigned int i = 0;
		bool hitRes = false;

		while (i < path.size())
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				result += '_';
			}
			else
			{
				result += path[i];
			}
			++i;
		}

		return result;
	}

	std::string Asset::getAssetNameFromRaw(char* data)
	{
		std::string assetName;

		while (!beginsWith(data, cursorPosition, binaryDelimiter))
		{
			assetName += *(data + cursorPosition++);
		}

		return assetName;
	}

	std::unique_ptr<char[]> Asset::getAssetDataFromRaw(char* data, unsigned int& size, unsigned int& kPos)
	{
		unsigned int e = cursorPosition;
		while (!beginsWith(data, e, assetEnd))
		{
			++e;
		}

		size = e - cursorPosition;
		auto res = std::make_unique<char[]>(size);

		for (unsigned int i = 0; i < size; ++i)
		{
			res[i] = data[i + cursorPosition];
		}

		cursorPosition += size;

		return res;
	}

	void Asset::bindFilesystem(std::shared_ptr<Filesystem> filesystem)
	{
		this->filesystem = filesystem;
	}

	void Asset::bindStorage(std::shared_ptr<Storage> storage)
	{
		this->storage = storage;
	}

	void Asset::loadGlobalAssetPack()
	{
		if (!hasLoadedGlobalAssetPack)
		{
			decodeAssetPack(gobalAssetPackName);
			hasLoadedGlobalAssetPack = true;
		}
	}

	std::shared_ptr<ResourceBundle> Asset::getAssetBundle(std::string path)
	{
		const auto name = assetNameFromPath(path);

		if (assetPack.count(name) != 0)
		{
			return assetPack.at(name);

		}
		else
		{
			const auto data = filesystem->readContentsFromFile(path, true);
			if (!data.empty())
			{
				assetPack[name] = std::make_shared<ResourceBundle>(data);
				return assetPack[name];
			}
		}

		Logger::instance()->logFatal("Asset::Attempted to pull asset for name= " + name + ", this does not exist in asset pack");
		return nullptr;
	}

	//encode and write PNG to memory (std::vector) with libpng on C++
	static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length)
	{
		std::vector<unsigned char>* p = (std::vector<unsigned char>*)png_get_io_ptr(png_ptr);
		p->insert(p->end(), data, data + length);
	}

	void Asset::compressRawImageToPng(std::shared_ptr<ImageBundle> inBundle, std::shared_ptr<ImageBundle> outBundle)
	{
		outBundle->type = ImageBundleType::Error;
		if (inBundle == nullptr || outBundle == nullptr || inBundle->type != ImageBundleType::Raw)
		{
			Logger::instance()->logCritical("Asset::Failed to compress raw image data, invalid input");
			return;
		}

		outBundle->data.clear();

		png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!p)
		{
			Logger::instance()->logCritical("Asset::Failed to compress raw image data, failed to create PNG write structure");
			return;
		}

		png_infop info_ptr = png_create_info_struct(p);
		if (!info_ptr)
		{
			Logger::instance()->logCritical("Asset::Failed to compress raw image data, failed to create PNG info structure");
			png_destroy_write_struct(&p, NULL);
			return;
		}

		if (setjmp(png_jmpbuf(p)))
		{
			Logger::instance()->logCritical("Asset::Failed to compress raw image data, failed to set jump buffer");
			png_destroy_write_struct(&p, &info_ptr);
			return;
		}

		png_set_IHDR(p, info_ptr, (png_uint_32)inBundle->width, (png_uint_32)inBundle->height, 8,
			PNG_COLOR_TYPE_RGB,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);
		std::vector<unsigned char*> rows(inBundle->height);
		for (size_t y = 0; y < inBundle->height; ++y)
			rows[y] = (unsigned char*)&inBundle->data[y * inBundle->width * 3];
		png_set_rows(p, info_ptr, &rows[0]);
		png_set_write_fn(p, &outBundle->data, PngWriteCallback, NULL);
		png_write_png(p, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

		outBundle->width = inBundle->width;
		outBundle->height = inBundle->height;
		outBundle->type = AW::ImageBundleType::Png;

		png_destroy_write_struct(&p, &info_ptr);
	}

	//encode and write PNG to memory (std::vector) with libpng on C++
	static void PngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		std::vector<unsigned char>* p = (std::vector<unsigned char>*)png_get_io_ptr(png_ptr);
		if (p == nullptr)
		{
			return;
		}

		for (int i = 0; i < length; ++i)
		{
			*(data + i) = (*p)[i];
		}

		p->erase(p->begin(), p->begin() + length);
	}

	void Asset::uncompressPngImageToRaw(std::shared_ptr<ImageBundle> inBundle, std::shared_ptr<ImageBundle> outBundle)
	{
		outBundle->type = ImageBundleType::Error;
		if (inBundle == nullptr || outBundle == nullptr || inBundle->type != ImageBundleType::Png)
		{
			Logger::instance()->logCritical("Asset::Failed to uncompress png image data, invalid input");
			return;
		}

		png_structp p = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!p)
		{
			Logger::instance()->logCritical("Asset::Failed to uncompress png image data, failed to create PNG write structure");
			return;
		}

		png_infop info_ptr = png_create_info_struct(p);
		if (!info_ptr)
		{
			Logger::instance()->logCritical("Asset::Failed to uncompress png image data, failed to create PNG info structure");
			png_destroy_read_struct(&p, NULL, NULL);
			return;
		}

		if (setjmp(png_jmpbuf(p)))
		{
			Logger::instance()->logCritical("Asset::Failed to uncompress png image data, failed to set jump location");
			png_destroy_read_struct(&p, &info_ptr, NULL);
			return;
		}

		png_set_read_fn(p, &inBundle->data, PngReadCallback);

		png_read_info(p, info_ptr);

		png_uint_32 width = 0;
		png_uint_32 height = 0;
		int bitDepth = 0;
		int colorType = -1;
		png_uint_32 channels = png_get_channels(p, info_ptr);
		png_uint_32 color_type = png_get_color_type(p, info_ptr);
		png_uint_32 retval = png_get_IHDR(p, info_ptr,
			&width,
			&height,
			&bitDepth,
			&colorType,
			NULL, NULL, NULL);

		auto rowPtrs = new png_bytep[height];

		auto size = width * height * bitDepth * channels / 8;
		auto inBuff = new char[size];
		const unsigned int stride = width * bitDepth * channels / 8;

		for (size_t i = 0; i < height; i++)
		{
			png_uint_32 q = (png_uint_32)(i)*stride;
			rowPtrs[i] = (png_bytep)inBuff + q;
		}

		png_read_image(p, rowPtrs);

		outBundle->data = std::vector<unsigned char>(inBuff, inBuff + size);
		outBundle->width = width;
		outBundle->height = height;
		outBundle->type = ImageBundleType::Raw;

		delete[](png_bytep)rowPtrs;
		delete[] inBuff;

		png_destroy_read_struct(&p, &info_ptr, NULL);
	}
}
