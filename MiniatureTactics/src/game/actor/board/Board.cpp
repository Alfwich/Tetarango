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
		return blockPtr == nullptr ? 0 : calcMapOffset(blockPtr->blockX, blockPtr->blockY);
	}

	int Board::calcMapOffset(unsigned int x, unsigned int y)
	{
		return x + y * 10000;
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

	std::shared_ptr<MT::SerializationClient> Board::doSerialize(MT::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__board__", hint);

		boardWidth = (unsigned int)client->serializeInt("board-width", boardWidth);
		boardHeight = (unsigned int)client->serializeInt("board-height", boardHeight);
		cellWidth = (unsigned int)client->serializeInt("cell-width", cellWidth);
		cellHeight = (unsigned int)client->serializeInt("cell-height", cellHeight);

		return MT::Element::doSerialize(hint);
	}

	void Board::onEnterFrame(double deltaTime)
	{
		if (actionTimer->isAboveThresholdAndRestart(250))
		{
			transitions.clear();
			outgoingBlocks.clear();
			for (unsigned int y = 0; y <= boardHeight; ++y)
			{
				auto rowFilled = true;
				for (unsigned int x = 0; x <= boardWidth; ++x)
				{
					auto mapping = calcMapOffset(x, y);
					auto nextMapping = calcMapOffset(x, y + 1);
					rowFilled = blockMap.count(mapping) == 1 && (y + 1 > boardHeight || blockMap.count(nextMapping) == 1);

					if (!rowFilled)
					{
						break;
					}
				}

				if (rowFilled)
				{
					for (unsigned int x = 0; x <= boardWidth; ++x)
					{
						outgoingBlocks.push_back(blockMap[calcMapOffset(x, y)]);
					}
				}
			}

			for (const auto outgoingBlock : outgoingBlocks)
			{
				blockMap.erase(calcMapOffset(outgoingBlock->blockX, outgoingBlock->blockY));

				const auto t = modules->animation->createGameTransition();
				t->startTransition(outgoingBlock, 50.0, MT::Rect(outgoingBlock->blockX * cellWidth + MT::NumberHelper::random(-200, 200), outgoingBlock->blockY * cellHeight + MT::NumberHelper::random(-200, 200), 0, 0));
				transitions.push_back(t);

				outgoingBlock->blockX = -1;
				outgoingBlock->blockY = -1;
			}

			for (const auto child : getChildrenOfType<Block>())
			{
				if (child->blockX == -1 || child->blockY == -1)
				{
					if (child->getWidth() == 0 || child->getHeight() == 0)
					{
						child->removeFromParent();
					}
					continue;
				}

				if (child->blockY < boardHeight) {
					const auto oldMapping = calcMapOffset(child);
					child->blockY += 1;
					const auto newMapping = calcMapOffset(child);
					if (blockMap.count(newMapping) == 0)
					{
						blockMap.erase(oldMapping);
						blockMap[newMapping] = child;

						const auto t = modules->animation->createGameTransition();
						t->startTransition(child, 50.0, MT::Rect(child->blockX * cellWidth, child->blockY * cellHeight, cellHeight, cellWidth));
						transitions.push_back(t);
					}
					else
					{
						child->blockY -= 1;
						positionBlock(child);
					}
				}
			}
		}
	}
}