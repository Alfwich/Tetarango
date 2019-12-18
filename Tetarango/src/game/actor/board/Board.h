#pragma once

#include <memory>
#include "ui/renderable/element/NineSlice.h"
#include "ui/renderable/element/Element.h"
#include "engine/module/SystemModuleBundle.h"
#include "prop/block/Block.h"

namespace AWGame
{
	class Board : public AW::Element
	{
		bool isFastFalling = false, hasFailedToPlacePiece = false, isFallingEnabled = true;
		unsigned int boardWidth = 10, boardHeight = 10, cellWidth = 32, cellHeight = 32;
		std::vector<std::shared_ptr<AW::Transition>> transitions;
		std::vector<std::shared_ptr<Block>> currentBlocks;
		std::vector<std::shared_ptr<Block>> eliminiatedBlocks;
		std::map<int, std::shared_ptr<Block>> blockMap;
		std::shared_ptr<AW::Timer> actionTimer;
		std::shared_ptr<AW::NineSlice> background;

		std::shared_ptr<AW::Transition> getTransition();
		void updateBoardIfNeeded();
		int calcMapOffset(const std::shared_ptr<Block> blockPtr);
		int calcMapOffset(unsigned int x, unsigned int y);
		void positionBlock(const std::shared_ptr<Block> blockPtr);

	public:
		Board();
		Board(unsigned int width, unsigned int height);

		void onLoadResources();

		void setCellSize(unsigned int width, unsigned int height);
		void addTetromino(const std::vector<std::shared_ptr<Block>>& blocks);
		std::vector<std::shared_ptr<Block>> getCurrentBlock();
		void moveCurrentPiece(int x, int y = 0);
		void rotatePiece();
		void enableFastFall();
		void disableFastFall();
		void enableBoardFalling();
		void disableBoardFalling();

		bool hasAnActivePiece();
		bool getHasFailedToPlacePiece();
		std::vector<std::shared_ptr<Block>> getEliminatedPieces();
		void resetBoard();

		void add(std::shared_ptr<GameObject> ao);
		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		virtual std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);
		void onEnterFrame(double deltaTime);
	};

}
