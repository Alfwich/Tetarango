#include "Logger.h"

namespace AWCore
{
	Logger* Logger::staticLoggerInstance = nullptr;

	Logger::Logger()
	{
		if (Logger::staticLoggerInstance == nullptr)
		{
			Logger::staticLoggerInstance = this;
			cachedLogMessages.push_back(processLogMessage("#################################################################"));
			cachedLogMessages.push_back(processLogMessage("###         Start Application on " + getCurrentDate("%d-%m-%Y, %H:%M:%S")) + "         ###");
			cachedLogMessages.push_back(processLogMessage("#################################################################"));
		}
		else
		{
			Logger::staticLoggerInstance->log("Logger::Logger was created when there was already an instance. Use Logger::instance() to get the instance.");
		}

		lock = SDL_CreateSemaphore(1);
	}

	Logger::~Logger()
	{
		SDL_DestroySemaphore(lock);
		Logger::staticLoggerInstance = nullptr;
	}

	Logger* Logger::instance()
	{
		if (Logger::staticLoggerInstance == nullptr)
		{
			std::cout << "Logger::Cannot find logger instance" << std::endl;
		}

		return Logger::staticLoggerInstance;
	}

	void Logger::bindFilesystem(std::shared_ptr<Filesystem> filesystem)
	{
		this->filesystem = filesystem;
	}

	void Logger::bindTime(std::shared_ptr<Time> time)
	{
		this->time = time;
		purgeCacheTimer = time->createPureTimer();
		purgeCacheTimer->start();
	}

	void Logger::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	void Logger::setLogFileName(std::string fileName)
	{
		logFileName = fileName;
		log("Logger::Changing log file to fileName=" + fileName);
	}

	void Logger::log(std::string msg, std::string prefix)
	{
		std::stringstream ss;
		ss << processLogMessage(msg, prefix) << std::endl;
		writeConsole(ss.str());
		writeToFile(ss.str());
	}

	void Logger::logCritical(std::string msg)
	{
		log(msg, "!! critical !!");

		if (throwOnCritical)
		{
			throw msg;
		}
	}

	void Logger::logCritical(char* msg)
	{
		logCritical(std::string(msg));
	}

	void Logger::logFatal(std::string msg)
	{
		log(msg, "!! FATAL !!");
		this->purgeToLogFile(true);
		throw msg;
	}

	void Logger::logFatal(char * msg)
	{
		logFatal(std::string(msg));
	}

	void Logger::log(char* msg, std::string prefix)
	{
		log(std::string(msg), prefix);
	}

	std::string Logger::convertAndPad(unsigned int i, const size_t num, const char paddingChar)
	{
		std::string str = std::to_string(i);
		if (num > str.size())
			str.insert(0, num - str.size(), paddingChar);
		return str;
	}

	void Logger::onInit()
	{
		purgeCacheTimer = time->createPureTimer();
		logToConsoleEnabled = gameConfig->getConfigBool(Config::Param::logToConsole);
		logToFileEnabled = gameConfig->getConfigBool(Config::Param::logToFile);
		throwOnCritical = gameConfig->getConfigBool(Config::Param::throwOnCriticalLog);
	}

	void Logger::onCleanup()
	{
		purgeToLogFile();
		logToFileEnabled = false;
	}

	void Logger::writeConsole(std::string msg)
	{
		if (logToConsoleEnabled)
		{
			std::cout << msg;
		}
	}

	void Logger::writeToFile(std::string msg)
	{
		if (filesystem == nullptr)
		{
			return;
		}

		if (!logToFileEnabled)
		{
			return;
		}

		SDL_SemWait(lock);
		cachedLogMessages.push_back(msg);
		SDL_SemPost(lock);

		if (thread->isMainThread() && filesystem->isReadyToLog() && purgeCacheTimer != nullptr)
		{
			if (purgeCacheTimer->getTicks() > msBetweenLogFilePurges)
			{
				purgeToLogFile();
			}
		}
	}

	void Logger::purgeToLogFile(bool immediate)
	{
		if (!logToFileEnabled)
		{
			return;
		}

		const auto purgeMsg = processLogMessage("Logger::Purging log to file=" + logFileName + "\n");
		writeConsole(purgeMsg);
		cachedLogMessages.push_back(purgeMsg);

		std::stringstream ss;
		for (const auto cachedMsg : cachedLogMessages)
		{
			ss << cachedMsg << "\r\n";
		}

		cachedLogMessages.clear();
		purgeCacheTimer->start();

		if (immediate)
		{
			filesystem->appendContentToFile(logFileName, ss.str());
		}
		else
		{
			filesystem->appendContentToFileAsync(logFileName, ss.str());
		}
	}

	std::string Logger::processLogMessage(std::string msg, std::string prefix)
	{
		return '[' + getTimestamp() + "] " + '[' + prefix + "] " + msg;
	}

	std::string Logger::getCurrentDate(std::string format)
	{
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);
		timeinfo = std::localtime(&rawtime);

		std::strftime(buffer, sizeof(buffer), format.c_str(), timeinfo);
		std::string str(buffer);

		return str;
	}

	std::string Logger::getTimestamp()
	{
		auto time = SDL_GetTicks();
		unsigned int hours = time / 3600000;
		time -= hours * 3600000;
		unsigned int mins = time / 60000;
		time -= mins * 60000;
		unsigned int secs = time / 1000;
		time -= secs * 1000;

		return convertAndPad(hours, 2, '0')
			+ ':' + convertAndPad(mins, 2, '0')
			+ ':' + convertAndPad(secs, 2, '0')
			+ ':' + convertAndPad(time, 4, '0');
	}
}

