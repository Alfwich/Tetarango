#pragma once

namespace AWGame
{
	class INotifyOnFade
	{
	public:
		virtual void onFadeOut() = 0;
		virtual void onFadeIn() = 0;
	};
}