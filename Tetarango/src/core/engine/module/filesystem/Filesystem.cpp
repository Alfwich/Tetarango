#include "Filesystem.h"

namespace AW
{
	void Filesystem::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	bool Filesystem::isReadyToLog()
	{
		return sdlPrefPath.size() > 0;
	}

	class FileReferenceWithAutoClose
	{
		SDL_RWops* const fileRef;

	public:
		FileReferenceWithAutoClose(const std::string& path, const std::string& mode)
			: fileRef(SDL_RWFromFile(path.c_str(), mode.c_str()))
		{
		}

		~FileReferenceWithAutoClose()
		{
			if (fileRef != NULL
				&& SDL_RWclose(fileRef) != 0)
			{
				Logger::instance()->logCritical("Filesystem::Error closing file stream");
			}
		}

		SDL_RWops* const get()
		{
			return fileRef;
		}
	};

	std::string Filesystem::readContentsFromFile(std::string path, bool isLocal)
	{
		const auto resPath = isLocal ? path : getAbsolutePathFromRelativePath(path);
		FileReferenceWithAutoClose fileRef(resPath, "r");

		if (fileRef.get() != NULL)
		{
			const Sint64 fileSizeOrRetCode = SDL_RWsize(fileRef.get());
			if (fileSizeOrRetCode >= 0)
			{
				const size_t fileSize = (size_t)fileSizeOrRetCode; // there might be an issue here if the file is so large that its amount of bytes can't fit in size_t
				const auto str = std::make_unique<char[]>(fileSize + 1);

				char* curPos = str.get();
				size_t amountReadSoFar = 0;
				size_t amountReadThisTime = 1;
				SDL_ClearError();
				for (; amountReadSoFar < fileSize && amountReadThisTime != 0;
					amountReadSoFar += amountReadThisTime, curPos += amountReadThisTime)
				{
					amountReadThisTime = SDL_RWread(fileRef.get(), curPos, 1, (fileSize - amountReadSoFar));
				}

				if (strlen(SDL_GetError()) == 0
					&& amountReadSoFar == fileSize)
				{
					unsigned int i = 0;
					std::stringstream ss;

					while (i < fileSize)
					{
						ss << *(str.get() + i++);
					}

					return ss.str();
				}
			}
		}

		Logger::instance()->log("Filesystem::Failed to read file on path=" + path + ", the file could not be opened");
		SDL_ClearError();

		return std::string();
	}

	WORKER_ID Filesystem::readContentsFromFileAsync(std::string path, std::weak_ptr<INotifyOnCompletion> callback)
	{
		if (thread->isCleanedUp)
		{
			readContentsFromFile(path);
			return -1;
		}

		auto thisServicePtr = std::static_pointer_cast<Filesystem>(shared_from_this());
		auto bundle = std::make_shared<AsyncOperationBundle<Filesystem, std::string>>(thisServicePtr, std::make_shared<std::string>(path));

		const auto workerId = thread->doWorkSharedPtr<std::string, AsyncOperationBundle<Filesystem, std::string>>(bundle,
			[](std::shared_ptr<AsyncOperationBundle<Filesystem, std::string>> bundle) -> std::shared_ptr<std::string>
			{
				try
				{
					const auto opBundle = bundle->data;
					const auto service = bundle->service;
					const auto result = service->readContentsFromFile(*bundle->data);
					return std::make_shared<std::string>(result);
				}
				catch (...)
				{
					return nullptr;
				}
			}
		, callback, WorkerTaskCode::FILE_READ);

		return workerId;
	}

	bool Filesystem::appendOrWriteContentToFile(const std::string& path, const std::string& content, const std::string& appendOrWrite)
	{
		FileReferenceWithAutoClose fileRef(getAbsolutePathFromRelativePath(path), appendOrWrite);
		if (fileRef.get() != NULL)
		{
			const size_t numObjsToWrite = content.size();
			if (SDL_RWwrite(fileRef.get(), content.c_str(), 1, numObjsToWrite) == numObjsToWrite)
			{
				return true;
			}
		}

		Logger::instance()->log("Filesystem::Failed to write or append (" + appendOrWrite + ") file on path=" + getAbsolutePathFromRelativePath(path) + ", file could not be opened");
		return false;
	}

	bool Filesystem::writeContentToFile(std::string path, const std::string& content)
	{
		return appendOrWriteContentToFile(path, content, "w");
	}

	bool Filesystem::appendContentToFile(std::string path, const std::string& content)
	{
		return appendOrWriteContentToFile(path, content, "a");
	}

	WORKER_ID Filesystem::appendContentToFileAsync(std::string path, std::string content, std::weak_ptr<INotifyOnCompletion> callback)
	{
		if (thread->isCleanedUp)
		{
			appendContentToFile(path, content);
			return -1;
		}

		auto thisServicePtr = std::static_pointer_cast<Filesystem>(shared_from_this());
		auto bundle = std::make_shared<AsyncOperationBundle<Filesystem, FileOpBundleData>>(thisServicePtr, std::make_shared<FileOpBundleData>(path, content));

		const auto workerId = thread->doWorkSharedPtr<bool, AsyncOperationBundle<Filesystem, FileOpBundleData>>(bundle,
			[](std::shared_ptr<AsyncOperationBundle<Filesystem, FileOpBundleData>> bundle) -> std::shared_ptr<bool>
			{
				try
				{
					const auto opBundle = bundle->data;
					const auto service = bundle->service;
					const auto result = service->appendContentToFile(opBundle->path, opBundle->content);
					return std::make_shared<bool>(result);
				}
				catch (...)
				{
					return nullptr;
				}
			}
		, callback, WorkerTaskCode::FILE_APPEND);

		return workerId;
	}

	Filesystem::FilesystemException::FilesystemException(const std::string& incMsg)
		: msg(incMsg)
	{
	}

	const char* Filesystem::FilesystemException::what() const noexcept
	{
		return msg.c_str();
	}

	std::string Filesystem::getAbsolutePathFromRelativePath(const std::string& path)
	{
		if (sdlPrefPath.empty() && gameConfig != nullptr)
		{
			char* incomingSdlPrefPath =
				SDL_GetPrefPath(
					gameConfig->getConfigString(Config::Param::organizationName).c_str(),
					gameConfig->getConfigString(Config::Param::storageLocation).c_str());

			if (incomingSdlPrefPath != NULL)
			{
				sdlPrefPath = incomingSdlPrefPath;

				SDL_free(incomingSdlPrefPath);
			}
			else
			{
				Logger::instance()->logCritical("Filesystem::Could not get SDL_GetPrefPath");
			}

		}

		return sdlPrefPath + path;
	}
}
