#include "Time.h"

namespace AW
{
	void Time::createTimeScope(TimeScope scopeName, double timeFactor)
	{
		if (timeScopes.count(scopeName) != 0)
		{
			Logger::instance()->logCritical("Time::Failed to create timeScope for scopeName=" + std::to_string((int)scopeName) + ", scope already exists");
			return;
		}

		timeScopes[scopeName] = std::make_unique<TimeScopeBundle>(timeFactor);
		Logger::instance()->log("Time::Created timeScope with name=" + std::to_string((int)scopeName) + ", and initial timeFactor=" + std::to_string(timeFactor));
	}

	void Time::changeTimeFactorForScope(TimeScope scopeName, double timeFactor)
	{
		if (timeScopes.count(scopeName) == 0)
		{
			Logger::instance()->logCritical("Time::Failed to update timeFactor for scopeName=" + std::to_string((int)scopeName) + " to timeFactor:" + std::to_string(timeFactor) + ", scope does not exist");
			return;
		}

		if (timeFactor < 0)
		{
			Logger::instance()->logCritical("Time::Failed to update timeFactor for scopeName=" + std::to_string((int)scopeName) + " to timeFactor:" + std::to_string(timeFactor) + ", negative timeFactor passed in");
			return;
		}

		timeScopes[scopeName]->timeFactor = timeFactor;
		Logger::instance()->log("Time::Changed timeFactor for scope=" + std::to_string((int)scopeName) + ", to timeFactor=" + std::to_string(timeFactor));
	}

	double Time::getTimeFactorForScope(TimeScope scopeName)
	{
		if (timeScopes.count(scopeName) == 0)
		{
			Logger::instance()->logCritical("Time::Failed to return timeFactor for scopeName=" + std::to_string((int)scopeName) + ", scope does not exist");
			return 1.0;
		}

		return timeScopes.at(scopeName)->timeFactor;
	}

	void Time::changeGlobalTimeFactor(double timeFactor)
	{
		changeTimeFactorForScope(TimeScope::Global, timeFactor);
	}

	double Time::getGlobalTimeFactor()
	{
		return getTimeFactorForScope(TimeScope::Global);
	}

	double Time::getComputedTimeFactor(TimeScope compositeScopeName)
	{
		return getTimeFactorForScope(compositeScopeName) * getGlobalTimeFactor();
	}

	bool Time::hasTimeScope(TimeScope scopeName)
	{
		return timeScopes.count(scopeName) == 1;
	}

	Uint32 Time::getTicks()
	{
		return SDL_GetTicks();
	}

	double Time::getFrameStartTime()
	{
		return frameStartTime;
	}

	double Time::getHighResolutionTicks()
	{
		return (SDL_GetPerformanceCounter() - startHighPerformancePosition) / (double)SDL_GetPerformanceFrequency() * 1000.0;
	}

	double Time::getDeltaTime()
	{
		return frameTime;
	}

	void Time::delay(Uint32 ms)
	{
		SDL_Delay(ms);
	}

	void Time::spinlock(double startHighResolutionTicks, double targetTime)
	{
		while (getHighResolutionTicks() - startHighResolutionTicks < targetTime) {}
	}

	std::shared_ptr<Timer> Time::createTimer()
	{
		return std::make_unique<Timer>(this, TimeScope::Global, false, false);
	}

	std::shared_ptr<Timer> Time::createTimer(TimeScope scopeName, bool isUnstoppable)
	{
		return std::make_unique<Timer>(this, scopeName, false, isUnstoppable);
	}

	std::shared_ptr<Timer> Time::createPureTimer()
	{
		return std::make_unique<Timer>(this, TimeScope::Global, true, false);
	}

	void Time::onInit()
	{
		startHighPerformancePosition = SDL_GetPerformanceCounter();
		createTimeScope(TimeScope::Global, 1.0);
		createTimeScope(TimeScope::ApplicationFrameTimer, 1.0);
	}

	void Time::onEnterFrame()
	{
		const auto currentFrameTime = getHighResolutionTicks();
		frameTime = currentFrameTime - frameStartTime;
		frameStartTime = currentFrameTime;
	}

}
