#pragma once

#include <string>
#include "engine/module/logger/Logger.h"
#include "engine/module/IBaseModule.h"
#include "engine/module/thread/Thread.h"

namespace AWCore
{
	class Filesystem : public IBaseModule
	{
		std::shared_ptr<Thread> thread;

		std::string sdlPrefPath;

		std::string getAbsolutePathFromRelativePath(const std::string& path);
		bool appendOrWriteContentToFile(const std::string& path, const std::string& content, const std::string& appendOrWrite);

	public:

		void bindThread(std::shared_ptr<Thread> thread);

		bool isReadyToLog();
		std::string readContentsFromFile(std::string path, bool isLocal = false);
		WORKER_ID readContentsFromFileAsync(std::string path, std::weak_ptr<INotifyOnCompletion> callback = std::weak_ptr<INotifyOnCompletion>());
		bool writeContentToFile(std::string path, const std::string& content);
		bool appendContentToFile(std::string path, const std::string& content);
		WORKER_ID appendContentToFileAsync(std::string path, std::string content, std::weak_ptr<INotifyOnCompletion> callback = std::weak_ptr<INotifyOnCompletion>());

		class FilesystemException : std::exception
		{
			const std::string msg;

		public:
			FilesystemException(const std::string&);
			virtual const char* what() const noexcept;
		};

		class FileOpBundleData
		{
		public:
			FileOpBundleData(std::string path, std::string content)
			{
				this->path = path;
				this->content = content;
			}

			std::string path;
			std::string content;
		};

	};

}
