#include "Animation.h"

namespace AW
{

	void Animation::addAnimationFrame(int x, int y, int width, int height)
	{
		std::shared_ptr<Rect> r = std::make_shared<Rect>();
		r->x = x;
		r->y = y;
		r->w = width;
		r->h = height;
		animationRects.push_back(r);
	}

	void Animation::addGeneralFrames(int startX, int startY, int width, int height, int numFrames, int sourceWidth, int sourceHeight)
	{
		int cX = startX, cY = startY, framesAdded = 0;
		while (cY < sourceHeight && framesAdded < numFrames)
		{
			std::shared_ptr<Rect> r = std::make_shared<Rect>();
			r->x = cX;
			r->y = cY;
			r->w = width;
			r->h = height;
			animationRects.push_back(r);
			framesAdded++;

			cX += width;
			if (cX >= sourceWidth)
			{
				cX = 0;
				cY += height;
			}
		}
	}

	std::shared_ptr<Rect> Animation::getFrameRect(int index)
	{
		return animationRects[index % animationRects.size()];
	}

	bool Animation::frameInAnimation(int index)
	{
		return index < (int)animationRects.size();
	}

	void Animation::setFps(int fps)
	{
		this->fps = fps;
	}

	int Animation::getFps()
	{
		return fps;
	}
}
