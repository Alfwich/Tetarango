#pragma once

#include "TimeScope.h"

namespace AW
{
	class Time;

	class Timer
	{
		bool running = false, paused = false, pure = false, scope = false, unstoppable = false;
		unsigned int currentTicks, pausedTicks;
		TimeScope timeScope;
		Time* time;

	public:
		Timer(Time* time, TimeScope timeScope, bool isPure, bool isUnstoppable);

		bool isPure();
		bool isUnstoppable();

		void start();
		void startIfNotRunning();
		void stop();
		void pause();
		bool isPaused();
		bool isRunning();
		double getTicks();
		double getTicksAndRestart();

		bool isAboveThresholdAndRestart(unsigned int threshold);
		bool isAboveThresholdAndStop(unsigned int threshold);

		void setTimeScope(TimeScope scopeName);
		void clearTimeScope();
		double getTimeFactor();
		double getScopeTimeFactor();
	};

}

#include "Time.h"
