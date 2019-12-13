#pragma once

namespace MT
{
	class INotifyOnTransition
	{
	public:
		virtual void onTransitionAnimationFrame(double position, int transitionId) { /* NO-OP*/ };
		virtual void onTransitionCompleted(int transitionId) { /* NO-OP*/ };
	};
}
