#pragma once

namespace AW
{
	class EnterFrameListener
	{
	public:

		bool enterFrameActivated = false;
		int enterFramePriority = 0;

		virtual int getObjectIdEnterFrame() = 0;

		virtual void enterFrame(const double& frameTime)
		{
			if (this->enterFrameActivated) this->onEnterFrame(frameTime);
		};

		virtual void onEnterFrame(const double& frameTime) { /* NO-OP */ };
		virtual void onTimeoutCalled(int id) { onTimeoutCalled(); };
		virtual void onTimeoutCalled() { /* NO-OP */ };
	};
}
