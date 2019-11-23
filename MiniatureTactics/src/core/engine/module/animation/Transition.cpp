#include "Transition.h"


namespace MT
{


	Transition::Transition(std::shared_ptr<Time> time, TimeScope scopeName)
	{
		id = ApplicationObject::getNextId();
		this->scopeName = scopeName;
		this->time = time;
	}

	int Transition::getObjectIdEnterFrame()
	{
		return id;
	}

	void Transition::startTransition(std::shared_ptr<Renderable> target, double duration, Rect endRect, double targetAlpha)
	{
		startRect = target->getRect();
		startAlpha = target->getAlpha();
		if (startRect == endRect && startAlpha == targetAlpha)
		{
			return;
		}

		if (playing)
		{
			stop();
		}

		if (!paused)
		{
			position = 0.0;
			this->duration = duration;
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
			targetAO = std::dynamic_pointer_cast<ApplicationObject>(target);
		}

		paused = false;
		playing = true;
		enterFrameActivated = true;
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

	void Transition::onEnterFrame(double frameTime)
	{
		if (playing && target != nullptr)
		{
			if (targetAO != nullptr && !targetAO->isActive())
			{
				return;
			}

			position += frameTime * time->getTimeFactorForScope(scopeName);

			if (position >= duration)
			{
				target->setSizeAndPosition(endRect);
				target->setAlpha(endAlpha);

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

				const auto listenerPtr = listener.lock();
				if (listenerPtr != nullptr)
				{
					listenerPtr->onTransitionCompleted();
				}
			}
			else
			{
				auto pos = position / duration;

				if (!(startRect == endRect))
				{
					auto intermediaryRect = Rect(startRect * (1.0 - pos) + (endRect * pos));
					target->setSizeAndPosition(intermediaryRect);
				}

				if (startAlpha != endAlpha)
				{
					auto intermediaryAlpha = startAlpha * (1.0 - pos) + endAlpha * pos;
					target->setAlpha(intermediaryAlpha);
				}
			}
		}
	}

}
