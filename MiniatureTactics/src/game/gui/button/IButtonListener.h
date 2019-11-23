#pragma once

#include <memory>

namespace MTGame
{
	class IButtonListener
	{
	public:
		virtual void onButtonClicked(int id) = 0;
	};
}
