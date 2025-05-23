#include "Timer.h"

namespace AW
{

	Timer::Timer(Time* time, TimeScope timeScope, bool isPure, bool isUnstoppable)
	{
		this->time = time;
		this->timeScope = timeScope;
		pure = isPure;
		unstoppable = isUnstoppable;
	}

	bool Timer::isPure()
	{
		return pure;
	}

	bool Timer::isUnstoppable()
	{
		return unstoppable;
	}

	void Timer::start()
	{
		if (paused)
		{
			unsigned int delta = time->getTicks() - pausedTicks;
			currentTicks += delta;
			paused = false;
		}
		else
		{
			currentTicks = time->getTicks();
			running = true;
		}
	}

	void Timer::startIfNotRunning()
	{
		if (running)
		{
			return;
		}

		start();
	}

	void Timer::stop()
	{
		if (unstoppable)
		{
			return;
		}

		currentTicks = 0;
		running = false;
	}

	void Timer::pause()
	{
		if (unstoppable)
		{
			return;
		}

		pausedTicks = time->getTicks();
		paused = true;
	}

	bool Timer::isPaused()
	{
		return paused;
	}

	bool Timer::isRunning()
	{
		return running;
	}

	double Timer::getTicks()
	{
		return (time->getTicks() - currentTicks) * getTimeFactor();
	}

	double Timer::getTicksAndRestart()
	{
		unsigned int currentTicks = time->getTicks();
		unsigned int result = currentTicks - this->currentTicks;
		this->currentTicks = currentTicks;
		return (result)*getTimeFactor();
	}

	bool Timer::isAboveThresholdAndStop(unsigned int threshold)
	{
		if (!running)
		{
			return false;
		}

		if (getTicks() >= threshold)
		{
			stop();
			return true;
		}

		return false;
	}

	bool Timer::isAboveThresholdAndRestart(unsigned int threshold)
	{
		if (!running)
		{
			return false;
		}

		if (getTicks() >= threshold)
		{
			start();
			return true;
		}

		return false;
	}

	void Timer::setTimeScope(TimeScope scopeName)
	{
		if (scopeName == TimeScope::None)
		{
			clearTimeScope();
			return;
		}

		if (!time->hasTimeScope(scopeName))
		{
			return;
		}

		timeScope = scopeName;
	}

	void Timer::clearTimeScope()
	{
		timeScope = TimeScope::None;
	}

	double Timer::getTimeFactor()
	{
		if (pure)
		{
			return 1.0;
		}

		return timeScope == TimeScope::None ? time->getGlobalTimeFactor() : time->getComputedTimeFactor(timeScope);
	}

	double Timer::getScopeTimeFactor()
	{
		if (pure)
		{
			return 1.0;
		}

		return timeScope == TimeScope::None ? 1.0 : time->getTimeFactorForScope(timeScope);
	}
}
