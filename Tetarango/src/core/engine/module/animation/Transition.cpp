#include "Transition.h"


namespace AW
{
	Transition::Transition(std::shared_ptr<Time> time, TimeScope scopeName)
	{
		id = GameObject::nextId();
		this->scopeName = scopeName;
		this->time = time;
	}

	int Transition::getObjectIdEnterFrame()
	{
		return id;
	}

	int Transition::getId()
	{
		return id;
	}

	void Transition::startTransition(std::shared_ptr<Renderable> target, double durationMS, Rect endRect, double targetAlpha)
	{
		startRect = target->getRect();
		startAlpha = target->getAlpha();
		const auto listenerPtr = getNotificationPtr();
		if (startRect == endRect && startAlpha == targetAlpha)
		{
			if (listenerPtr != nullptr)
			{
				listenerPtr->onTransitionStarted(id);
				listenerPtr->onTransitionCompleted(id);
			}

			return;
		}

		if (playing)
		{
			stop();
		}

		if (!paused)
		{
			position = 0.0;
			this->duration = durationMS;
			startRect = target->getRect();
			this->endRect = endRect;
			startAlpha = target->getAlpha();
			if (targetAlpha < 0)
			{
				endAlpha = startAlpha;
			}
			else
			{
				endAlpha = targetAlpha;
			}
			this->target = target;
			targetAO = std::dynamic_pointer_cast<GameObject>(target);
		}

		paused = false;
		playing = true;
		enterFrameActivated = true;

		target->onTransitionStart();
		if (listenerPtr != nullptr)
		{
			listenerPtr->onTransitionStarted(id);
		}

		if (duration == 0.0)
		{
			performTargetedFrameUpdate(0.0);
		}
	}

	void Transition::startTargetlessTransition(double durationMS)
	{
		position = 0.0;
		this->duration = durationMS;

		target = nullptr;
		paused = false;
		playing = true;
		enterFrameActivated = true;

		const auto listenerPtr = getNotificationPtr();
		if (listenerPtr != nullptr)
		{
			listenerPtr->onTransitionStarted(id);
		}

		if (duration == 0.0)
		{
			performTargetlessFrameUpdate(0.0);
		}
	}

	void Transition::resume()
	{
		if (target != nullptr)
		{
			paused = false;
			playing = true;
			enterFrameActivated = true;
		}
	}

	void Transition::pause()
	{
		paused = true;
		enterFrameActivated = false;
	}

	void Transition::stop()
	{
		playing = false;
		paused = false;
		enterFrameActivated = false;
		target = nullptr;
		targetAO = nullptr;
	}

	void Transition::finish()
	{
		position = duration;
	}

	bool Transition::isPaused()
	{
		return paused;
	}

	bool Transition::isPlaying()
	{
		return playing;
	}

	void Transition::setLooping(bool looping)
	{
		this->looping = looping;
	}

	void Transition::onEnterFrame(const double& frameTime)
	{
		if (playing)
		{
			if (target != nullptr)
			{
				performTargetedFrameUpdate(frameTime);
			}
			else
			{
				performTargetlessFrameUpdate(frameTime);
			}
		}
	}

	void Transition::performTargetedFrameUpdate(double frameTime)
	{
		if (targetAO != nullptr && !targetAO->isActive())
		{
			return;
		}

		position += frameTime * time->getTimeFactorForScope(scopeName);

		if (position >= duration)
		{
			target->onTransitionFrame(1.0, endRect, endAlpha, id);

			const auto listenerPtr = getNotificationPtr();
			if (listenerPtr != nullptr)
			{
				listenerPtr->onTransitionAnimationFrame(1.0, id);
			}

			if (looping)
			{
				position -= duration;
				auto start = startRect;
				startRect = endRect;
				endRect = start;
				auto startA = startAlpha;
				startAlpha = endAlpha;
				endAlpha = startA;
			}
			else
			{
				target->onTransitionEnd();
				stop();
			}

			if (listenerPtr != nullptr)
			{
				listenerPtr->onTransitionCompleted(id);
			}
		}
		else
		{
			auto pos = position / duration;

			auto intermediaryRect = Rect(startRect * (1.0 - pos) + (endRect * pos));
			auto intermediaryAlpha = startAlpha * (1.0 - pos) + endAlpha * pos;
			target->onTransitionFrame(pos, intermediaryRect, intermediaryAlpha, id);

			const auto listenerPtr = getNotificationPtr();
			if (listenerPtr != nullptr)
			{
				listenerPtr->onTransitionAnimationFrame(pos, id);
			}
		}
	}

	void Transition::performTargetlessFrameUpdate(double frameTime)
	{
		position += frameTime * time->getTimeFactorForScope(scopeName);

		if (position >= duration)
		{
			const auto listenerPtr = getNotificationPtr();
			if (listenerPtr != nullptr)
			{
				listenerPtr->onTransitionAnimationFrame(1.0, id);
			}

			if (looping)
			{
				position -= duration;
				auto start = startRect;
				startRect = endRect;
				endRect = start;
				auto startA = startAlpha;
				startAlpha = endAlpha;
				endAlpha = startA;
			}
			else
			{
				stop();
			}

			if (listenerPtr != nullptr)
			{
				listenerPtr->onTransitionCompleted(id);
			}
		}
		else
		{
			const auto listenerPtr = getNotificationPtr();
			if (listenerPtr != nullptr)
			{
				listenerPtr->onTransitionAnimationFrame(position / duration, id);
			}
		}
	}

	std::shared_ptr<INotifyOnTransition> Transition::getNotificationPtr()
	{
		const auto ptr = listener.lock();

		return ptr != nullptr ? std::dynamic_pointer_cast<INotifyOnTransition>(ptr) : nullptr;
	}
}
