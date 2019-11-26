#include "Board.h"
#include "ui/renderable/element/Rectangle.h"

namespace MTGame
{

	bool Board::isLoaded = false;

	void Board::updateBoardIfNeeded()
	{
		if (background != nullptr)
		{
			setSize(boardWidth * cellWidth, boardHeight * cellHeight);
			background->matchSizeAndCenter(this);
		}
	}

	int Board::calcMapOffset(const std::shared_ptr<Block> blockPtr)
	{
		return blockPtr == nullptr ? 0 : blockPtr->blockX + blockPtr->blockY * 100000;
	}

	void Board::positionBlock(const std::shared_ptr<Block> blockPtr)
	{
		if (blockPtr != nullptr)
		{
			blockPtr->setPosition(blockPtr->blockX * cellWidth, blockPtr->blockY * cellHeight);
		}
	}

	void Board::primeBoards()
	{
		std::make_unique<Board>();
	}

	void Board::loadResources(std::shared_ptr<MT::SystemModuleBundle> modules)
	{
		if (isLoaded) {
			return;
		}

		isLoaded = true;
	}

	Board::Board()
	{
		enableSerialization<Board>();
	}

	Board::Board(unsigned int boardWidth, unsigned int boardHeight) : Board()
	{
		this->boardWidth = boardWidth;
		this->boardHeight = boardHeight;
		updateBoardIfNeeded();
	}

	void Board::setCellSize(unsigned int width, unsigned int height)
	{
		cellWidth = width;
		cellHeight = height;
		updateBoardIfNeeded();
	}

	void Board::add(std::shared_ptr<ApplicationObject> ao)
	{
		const auto blockPtr = std::dynamic_pointer_cast<Block>(ao);
		if (blockPtr != nullptr)
		{
			const auto mapOffset = calcMapOffset(blockPtr);
			if (blockMap.count(calcMapOffset(blockPtr)) != 0)
			{
				// Cant add block due to collision
				MT::Logger::instance()->logCritical("Board > add: Trying to add block to position that is already occupied (" + std::to_string(mapOffset) + ")");
			}
			else
			{
				blockMap[mapOffset] = blockPtr;
				positionBlock(blockPtr);
				Element::add(blockPtr);
			}

		}
		else
		{
			Element::add(ao);
		}
	}

	void Board::onInitialAttach()
	{
		actionTimer = modules->time->createTimer(MT::TimeScope::Game);
		actionTimer->start();
		enableEnterFrame();
	}

	void Board::onCreateChildren()
	{
		background = std::make_shared<MT::Rectangle>();
		background->name = "background";
		background->zIndex = -2;
		updateBoardIfNeeded();
		background->setColor(255, 255, 255);
		add(background);
	}

	void Board::onChildrenHydrated()
	{
		background = findChildWithName<MT::Rectangle>("background");
	}

	void Board::onEnterFrame(double deltaTime)
	{
		if (actionTimer->isAboveThresholdAndRestart(250))
		{
			for (const auto child : getChildrenOfType<Block>())
			{
				if (child->blockY < boardHeight) {
					const auto oldMapping = calcMapOffset(child);
					child->blockY += 1;
					const auto newMapping = calcMapOffset(child);
					if (blockMap.count(newMapping) == 0)
					{
						blockMap.erase(oldMapping);
						blockMap[newMapping] = child;
					}
					else
					{
						child->blockY -= 1;
					}
				}
				positionBlock(child);
			}
		}
	}
}