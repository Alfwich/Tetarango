#pragma once

#include <memory>

namespace MTGame
{
	class IGuiListener
	{
	public:
		virtual void onButtonClicked(int id) { /* NO-OP */ };
	};
}
