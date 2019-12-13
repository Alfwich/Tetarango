#pragma once

#include <string>
#include <memory>
#include "engine/module/time/Time.h"
#include "engine/module/event/EnterFrameListener.h"
#include "util/Rect.h"
#include "INotifyOnTransition.h"

namespace MT
{
	class Renderable;
	class ApplicationObject;

	class Transition : public EnterFrameListener
	{
		std::shared_ptr<Time> time;

		int id;
		double position = 0.0, duration = 0.0;
		bool playing = false, paused = false, looping = false;
		TimeScope scopeName;
		Rect startRect;
		Rect endRect;

		double startAlpha = 0.0;
		double endAlpha = 0.0;

		std::shared_ptr<Renderable> target;
		std::shared_ptr<ApplicationObject> targetAO;

	public:
		Transition(std::shared_ptr<Time> time, TimeScope scopeName);

		int getObjectIdEnterFrame();
		int getId();

		std::weak_ptr<INotifyOnTransition> listener;

		void startTransition(std::shared_ptr<Renderable> target, double durationMS, Rect targetRect, double targetAlpha = -1.0);
		void resume();
		void pause();
		void stop();
		void finish();

		bool isPaused();
		bool isPlaying();

		void setLooping(bool looping);

		void onEnterFrame(double frameTime);
	};

}

#include "ui/renderable/Renderable.h"
#include "engine/ApplicationObject.h"
