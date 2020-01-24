#pragma once

#include "ui/renderable/element/Element.h"

namespace AWGame
{
	enum class BackdropType
	{
		Mountain1,
		Cloud
	};

	class BackdropObject : public AW::Element
	{
	public:
		BackdropObject();

		AWVec2<double> velocity;

		void onLoadResources() override;
		void onChildrenHydrated() override;

		void setBackdropType(BackdropType type);
	};
}
