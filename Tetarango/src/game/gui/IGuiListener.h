#pragma once

#include <memory>

namespace AWGame
{
	class IGuiListener
	{
	public:
		virtual void onButtonClicked(int id) { /* NO-OP */ };
		virtual void onScrollBarScroll(int id, double pos) { /* NO-OP */ };
	};
}
