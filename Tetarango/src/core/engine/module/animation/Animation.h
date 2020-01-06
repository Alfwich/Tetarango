#pragma once

#include "engine/module/time/Time.h"

namespace AW
{
	class Animation
	{
		std::vector<std::shared_ptr<Rect>> animationRects;
		int fps = 15;
	public:
		int getFps();
		void setFps(int fps);
		void addAnimationFrame(int x, int y, int width, int height);
		void addGeneralFrames(int startX, int startY, int frameWidth, int frameHeight, int numFrames = INT_MAX, int sourceWidth = INT_MAX, int sourceHeight = INT_MAX);
		std::shared_ptr<Rect> getFrameRect(int index);
		bool frameInAnimation(int index);
	};
}
