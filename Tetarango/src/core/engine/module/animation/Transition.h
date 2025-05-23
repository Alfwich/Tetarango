#pragma once

#include "engine/module/time/Time.h"
#include "engine/module/event/EnterFrameListener.h"
#include "INotifyOnTransition.h"

namespace AW
{
	class Renderable;
	class GameObject;

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

		std::shared_ptr<Renderable> target = nullptr;
		std::shared_ptr<GameObject> targetAO = nullptr;

		void performTargetedFrameUpdate(double frameTime);
		void performTargetlessFrameUpdate(double frameTime);

		std::shared_ptr<INotifyOnTransition> getNotificationPtr();

	public:
		Transition(std::shared_ptr<Time> time, TimeScope scopeName);

		int getObjectIdEnterFrame();
		int getId();

		std::weak_ptr<GameObject> listener;

		void startTransition(std::shared_ptr<Renderable> target, double durationMS, Rect targetRect, double targetAlpha = -1.0);
		void startTargetlessTransition(double durationMS);
		void resume();
		void pause();
		void stop();
		void finish();

		bool isPaused();
		bool isPlaying();

		void setLooping(bool looping);

		void onEnterFrame(const double& frameTime);
	};

}

#include "ui/renderable/Renderable.h"
#include "engine/GameObject.h"
