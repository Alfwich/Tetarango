#pragma once

#include <memory>

namespace MTGame
{
	class IGuiListener
	{
	public:
		virtual void onButtonClicked(int id) { /* NO-OP */ };

		virtual void onScrollBarScroll(double pos) { /* NO-OP */ };
		virtual double onScrollBarconstituentUpdated(double pos) { return 0.0; /* NO-OP */ };
	};
}
