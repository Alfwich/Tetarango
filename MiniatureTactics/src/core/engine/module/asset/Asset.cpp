#include "Asset.h"

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

	/*
	bool Asset::getAssetData(std::string path, char*& bufferLocation, int& bufferSize)
	{
		if (assetPack.empty())
		{
			return false;
		}

		const auto name = assetNameFromPath(path);

		if (assetPack.count(name) == 0)
		{
			Logger::instance()->logFatal("Asset::Attempted to pull asset for name= " + name + ", this does not exist in asset pack");
			return false;
		}

		const auto bundle = assetPack.at(name);

		bufferLocation = bundle->data.get();
		bufferSize = bundle->size;

		return true;
	}
	*/

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
}
