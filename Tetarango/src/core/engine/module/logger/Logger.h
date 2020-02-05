#pragma once

#include "engine/module/IBaseModule.h"

namespace AW
{
	class Filesystem;
	class Time;
	class Timer;
	class Thread;
	class Lua;

	class Logger : public IBaseModule
	{
		static Logger* staticLoggerInstance;

		SDL_sem* lock;
		bool logToFileEnabled = true, logToConsoleEnabled = true, throwOnCritical = false, throwOnFatal = false;
		std::vector<std::string> cachedLogMessages;
		std::string logFileName = "log.txt";
		double msBetweenLogFilePurges = 10000.0;

		std::shared_ptr<Time> time;
		std::shared_ptr<Filesystem> filesystem;
		std::shared_ptr<Thread> thread;
		std::shared_ptr<Timer> purgeCacheTimer;

		std::string prefix;

		std::string convertAndPad(unsigned int i, const size_t num, const char paddingChar = ' ');
		std::string getTimestamp();

		std::string processLogMessage(const std::string& msg, const std::string& prefix = "debug");
		void writeConsole(const std::string& msg);
		void writeToFile(const std::string& msg);
		void purgeToLogFile(bool immediate = false);
		std::string getCurrentDate(const std::string& format);

	public:
		static Logger* instance();
		Logger();
		virtual ~Logger();

		void bindFilesystem(std::shared_ptr<Filesystem> filesystem);
		void bindTime(std::shared_ptr<Time> time);
		void bindThread(std::shared_ptr<Thread> thread);

		void setLogFileName(std::string fileName);

		void log(const std::string& msg, const std::string& prefix = "debug");
		void log(char* msg, const std::string& prefix = "debug");
		void logCritical(const std::string& msg);
		void logCritical(char* msg);
		void logFatal(const std::string& msg);
		void logFatal(char* msg);

		void onInit() override;
		void onBindLuaHooks(const std::shared_ptr<Lua>& lua) override;
		void onCleanup() override;

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override;
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
	};

}

#include "engine/module/filesystem/Filesystem.h"
#include "engine/module/time/Timer.h"
#include "engine/module/time/Time.h"
#include "engine/module/thread/Thread.h"
#include "engine/module/lua/Lua.h"
