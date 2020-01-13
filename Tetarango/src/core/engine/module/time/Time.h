#pragma once

#include "engine/module/IBaseModule.h"
#include "Timer.h"
#include "TimeScope.h"

namespace AW
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
		Uint64 startHighPerformancePosition = 0;
		double frameStartTime = 0.0, frameTime = 0.0;

	public:
		void createTimeScope(TimeScope scopeName, double timeFactor);
		void changeTimeFactorForScope(TimeScope scopeName, double timeFactor);
		double getTimeFactorForScope(TimeScope scopeName);
		void changeGlobalTimeFactor(double timeFactor);
		double getGlobalTimeFactor();
		double getComputedTimeFactor(TimeScope compositeScopeName);
		bool hasTimeScope(TimeScope scopeName);

		Uint32 getTicks();
		double getFrameStartTime();
		double getHighResolutionTicks();

		double getDeltaTime();

		void delay(Uint32 ms);
		void spinlock(double startHighResolutionTicks, double targetTime);

		std::shared_ptr<Timer> createTimer();
		std::shared_ptr<Timer> createTimer(TimeScope scopeName, bool isUnstoppable = false);
		std::shared_ptr<Timer> createPureTimer();

		void onInit();
		void onEnterFrame();
	};

}
