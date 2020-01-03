#pragma once

#include "scene/BaseScene.h"

namespace AWGame
{
	class SceneWorldTetarango : public BaseScene
	{
	public:
		SceneWorldTetarango();

		void onAttach();

		void onCreateChildren();
		void onChildrenHydrated();
	};
}
