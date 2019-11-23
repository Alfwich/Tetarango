#pragma once

#include <memory>
#include <string>
#include <SDL.h>
#include "TimeScope.h"

namespace MT
{
	class Time;

	class Timer
	{
		bool running, paused, pure, scope, unstoppable;
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

		void setTimeScope(TimeScope scopeName);
		void clearTimeScope();
		double getTimeFactor();
		double getScopeTimeFactor();
	};

}
#include "Time.h"
