#pragma once

namespace MTGame
{
	class INotifyOnFade
	{
	public:
		virtual void onFadeOut() = 0;
		virtual void onFadeIn() = 0;
	};
}