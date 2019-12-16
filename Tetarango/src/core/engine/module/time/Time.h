#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <SDL.h>
#include "Timer.h"
#include "engine/module/IBaseModule.h"
#include "engine/module/logger/Logger.h"
#include "TimeScope.h"

namespace AWCore
{
	class TimeScopeBundle 
	{
	public:
		TimeScopeBundle(double timeFactor) { this->timeFactor = timeFactor; };
		double timeFactor;
	};

	class Time : public IBaseModule
	{
		std::unordered_map<TimeScope, std::shared_ptr<TimeScopeBundle>> timeScopes;

	public:
		void createTimeScope(TimeScope scopeName, double timeFactor);
		void changeTimeFactorForScope(TimeScope scopeName, double timeFactor);
		double getTimeFactorForScope(TimeScope scopeName);
		void changeGlobalTimeFactor(double timeFactor);
		double getGlobalTimeFactor();
		double getComputedTimeFactor(TimeScope compositeScopeName);
		bool hasTimeScope(TimeScope scopeName);

		std::shared_ptr<Timer> createTimer();
		std::shared_ptr<Timer> createTimer(TimeScope scopeName, bool isUnstoppable = false);
		std::shared_ptr<Timer> createPureTimer();

		void onInit();
	};

}
