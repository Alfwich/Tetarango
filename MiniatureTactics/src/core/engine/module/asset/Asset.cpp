#include "Asset.h"

#include <png.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "SDL.h"

namespace MT
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

	bool Asset::beginsWith(char * data, unsigned int pos, std::string tag)
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

	void Asset::onInit()
	{
		if (gameConfig->getConfigBool(Config::Param::useAssetPack))
		{
			decodeAssetPack(gameConfig->getConfigString(Config::Param::assetPackName));

			if (assetPack.empty())
			{
				Logger::instance()->logFatal("Asset::Failed to load asset pack");
			}
		}
	}

	std::shared_ptr<ResourceBundle> Asset::getAssetBundle(std::string path)
	{
		if (assetPack.empty())
		{
			return nullptr;
		}

		const auto name = assetNameFromPath(path);

		if (assetPack.count(name) == 0)
		{
			Logger::instance()->logFatal("Asset::Attempted to pull asset for name= " + name + ", this does not exist in asset pack");
			return false;
		}

		return assetPack.at(name);
	}

	//encode and write PNG to memory (std::vector) with libpng on C++

	//#define ASSERT_EX(cond, error_message) do { if (!(cond)) { std::cerr << error_message; exit(1);} } while(0)

	static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
		std::vector<unsigned char> *p = (std::vector<unsigned char>*)png_get_io_ptr(png_ptr);
		p->insert(p->end(), data, data + length);
	}

	struct TPngDestructor {
		png_struct *p;
		TPngDestructor(png_struct *p) : p(p) {}
		~TPngDestructor() { if (p) { png_destroy_write_struct(&p, NULL); } }
	};

	std::vector<unsigned char> Asset::compressRawImageToPng(size_t w, size_t h, char *data)
	{
		std::vector<unsigned char> result;
		png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!p)
		{
			Logger::instance()->logFatal("Asset::Failed to compress raw image data, failed to create PNG write structure");
			return result;
		}

		png_infop info_ptr = png_create_info_struct(p);
		if (!info_ptr)
		{
			Logger::instance()->logFatal("Asset::Failed to compress raw image data, failed to create PNG info structure");
			png_destroy_write_struct(&p, NULL);
			return result;
		}

		if (setjmp(png_jmpbuf(p)))
		{
			Logger::instance()->logFatal("Asset::Failed to compress raw image data, failed to set jump buffer");
			png_destroy_write_struct(&p, NULL);
			return result;
		}

		png_set_IHDR(p, info_ptr, (png_uint_32)w, (png_uint_32)h, 8,
			PNG_COLOR_TYPE_RGB,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);
		//png_set_compression_level(p, 1);
		std::vector<unsigned char*> rows(h);
		for (size_t y = 0; y < h; ++y)
			rows[y] = (unsigned char*)data + y * w * 3;
		png_set_rows(p, info_ptr, &rows[0]);
		png_set_write_fn(p, &result, PngWriteCallback, NULL);
		png_write_png(p, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
		png_destroy_write_struct(&p, NULL);

		return result;
	}
}
