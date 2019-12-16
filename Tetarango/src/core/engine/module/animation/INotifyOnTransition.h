#pragma once

namespace AWCore
{
	class INotifyOnTransition
	{
	public:
		virtual void onTransitionStarted(int transitionId) { onTransitionStarted(); };
		virtual void onTransitionStarted() { /* NO-OP*/ };
		virtual void onTransitionAnimationFrame(double position, int transitionId) { onTransitionAnimationFrame(position); };
		virtual void onTransitionAnimationFrame(double position) { onTransitionAnimationFrame(); };
		virtual void onTransitionAnimationFrame() { /* NO-OP*/ };
		virtual void onTransitionCompleted(int transitionId) { onTransitionCompleted(); };
		virtual void onTransitionCompleted() { /* NO-OP*/ };
	};
}
