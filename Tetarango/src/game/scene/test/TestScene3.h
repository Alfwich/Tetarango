#pragma once

#include "ui/scene/Scene.h"

namespace AWGame
{

	class TestScene3 : public AW::Scene
	{
	public:
		TestScene3();
		void onLoadResources() override;
		void onInitialAttach() override;
	};

}

