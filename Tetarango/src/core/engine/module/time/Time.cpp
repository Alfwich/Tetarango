#include "Time.h"

namespace AWCore
{

	void Time::onInit()
	{
		createTimeScope(TimeScope::Global, 1.0);
		createTimeScope(TimeScope::ApplicationFrameTimer, 1.0);
		createTimeScope(TimeScope::ApplicationFrameLimitTimer, 1.0);
	}

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
}
