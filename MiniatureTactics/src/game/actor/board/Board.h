#pragma once

#include <memory>
#include "ui/renderable/element/Element.h"
#include "engine/module/SystemModuleBundle.h"

namespace MTGame
{
	class Board : public MT::Element
	{
		static bool isLoaded;

	public:
		static void primeBoards();
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> modules);

		Board();

		void onInitialAttach();
		void onCreateChildren();
		void onEnterFrame(double deltaTime);
	};

}
