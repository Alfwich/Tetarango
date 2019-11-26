#pragma once

#include <memory>
#include "ui/renderable/element/Rectangle.h"
#include "ui/renderable/element/Element.h"
#include "engine/module/SystemModuleBundle.h"
#include "prop/block/Block.h"

namespace MTGame
{
	class Board : public MT::Element
	{
		static bool isLoaded;

		unsigned int boardWidth = 10, boardHeight = 10, cellWidth = 64, cellHeight = 64;
		std::map<int, std::shared_ptr<Block>> blockMap;
		std::shared_ptr<MT::Timer> actionTimer;
		std::shared_ptr<MT::Rectangle> background;

		void updateBoardIfNeeded();
		int calcMapOffset(const std::shared_ptr<Block> blockPtr);
		void positionBlock(const std::shared_ptr<Block> blockPtr);

	public:
		static void primeBoards();
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> modules);

		Board();
		Board(unsigned int width, unsigned int height);

		void setCellSize(unsigned int width, unsigned int height);

		void add(std::shared_ptr<ApplicationObject> ao);
		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onEnterFrame(double deltaTime);
	};

}
