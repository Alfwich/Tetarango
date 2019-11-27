#include "Board.h"
#include "ui/renderable/element/Rectangle.h"

namespace MTGame
{

	bool Board::isLoaded = false;

	std::shared_ptr<MT::Transition> Board::getTransition()
	{
		if (transitions.size() > 0)
		{
			auto result = transitions.back();
			transitions.pop_back();
			return result;
		}

		return modules->animation->createGameTransition();
	}

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

	void Board::enableFastFall()
	{
		isFastFalling = true;
	}

	void Board::disableFastFall()
	{
		isFastFalling = false;
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

	void Board::addTetromino(const std::vector<std::shared_ptr<Block>>& blocks)
	{
		currentBlocks.clear();
		const auto centerX = MT::NumberHelper::randomInt(1, boardWidth - 2);
		for (const auto block : blocks)
		{
			block->blockY += 2;
			block->blockX += centerX;
			add(block);
			currentBlocks.push_back(block);
		}
	}

	void Board::moveCurrentPiece(int x, int y)
	{
		auto shouldAllowMove = true;
		for (const auto currentPiece : currentBlocks)
		{
			const auto newBlockLocation = calcMapOffset(currentPiece->blockX + x, currentPiece->blockY + y);
			shouldAllowMove = shouldAllowMove
				&& (blockMap.count(newBlockLocation) == 0 || !blockMap[newBlockLocation]->hasSettled)
				&& currentPiece->blockX + x >= 0
				&& currentPiece->blockX + x <= (int)boardWidth
				&& currentPiece->blockY + y >= 0
				&& currentPiece->blockY + y <= (int)boardHeight;
		}

		if (shouldAllowMove)
		{
			for (const auto currentPiece : currentBlocks)
			{
				blockMap.erase(calcMapOffset(currentPiece));
			}

			for (const auto currentPiece : currentBlocks)
			{
				currentPiece->blockX += x;
				currentPiece->blockY += y;
				blockMap[calcMapOffset(currentPiece)] = currentPiece;

				const auto t = modules->animation->createGameTransition();
				t->startTransition(currentPiece, 50.0, MT::Rect(currentPiece->blockX * cellWidth, currentPiece->blockY * cellHeight, cellHeight, cellWidth));
				transitions.push_back(t);
			}
		}
	}

	void Board::rotatePiece()
	{
		if (currentBlocks.size() == 0)
		{
			return;
		}

		const auto rotationPoint = currentBlocks[0];
		const auto xOffset = rotationPoint->blockX;
		const auto yOffset = rotationPoint->blockY;

		bool shouldAllowRotation = true;
		for (const auto currentPiece : currentBlocks)
		{
			const auto shiftedX = currentPiece->blockX - xOffset;
			const auto shiftedY = currentPiece->blockY - yOffset;
			const auto shiftedRotatedX = -shiftedY;
			const auto shiftedRotatedY = shiftedX;
			const auto rotatedY = shiftedRotatedY + yOffset;
			const auto rotatedX = shiftedRotatedX + xOffset;
			const auto newBlockLocation = calcMapOffset(rotatedX, rotatedY);
			shouldAllowRotation = shouldAllowRotation
				&& (blockMap.count(newBlockLocation) == 0 || !blockMap[newBlockLocation]->hasSettled)
				&& rotatedX >= 0
				&& rotatedX <= (int)boardWidth
				&& rotatedY >= 0
				&& rotatedY <= (int)boardHeight;
		}

		if (shouldAllowRotation)
		{

			for (const auto currentPiece : currentBlocks)
			{
				blockMap.erase(calcMapOffset(currentPiece));
			}

			for (const auto currentPiece : currentBlocks)
			{
				const auto shiftedX = currentPiece->blockX - xOffset;
				const auto shiftedY = currentPiece->blockY - yOffset;
				const auto shiftedRotatedX = -shiftedY;
				const auto shiftedRotatedY = shiftedX;
				const auto rotatedY = shiftedRotatedY + yOffset;
				const auto rotatedX = shiftedRotatedX + xOffset;

				currentPiece->blockX = rotatedX;
				currentPiece->blockY = rotatedY;
				blockMap[calcMapOffset(currentPiece)] = currentPiece;

				const auto t = modules->animation->createGameTransition();
				t->startTransition(currentPiece, 50.0, MT::Rect(currentPiece->blockX * cellWidth, currentPiece->blockY * cellHeight, cellHeight, cellWidth));
				transitions.push_back(t);
			}
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
		if (actionTimer->isAboveThresholdAndRestart(isFastFalling ? 50 : 250))
		{
			for (auto it = activeTransitions.begin(); it != activeTransitions.end();)
			{
				const auto transition = (*it);
				if (!transition->isPlaying())
				{
					it = activeTransitions.erase(it);
					transitions.push_back(transition);
				}
				else
				{
					++it;
				}
			}

			outgoingBlocks.clear();
			for (unsigned int y = 0; y <= boardHeight; ++y)
			{
				auto rowFilled = true;
				for (unsigned int x = 0; x <= boardWidth; ++x)
				{
					auto nextCol = y + 1;
					auto mapping = calcMapOffset(x, y);
					rowFilled = blockMap.count(mapping) == 1;

					while (rowFilled && nextCol <= boardHeight)
					{
						rowFilled = blockMap.count(calcMapOffset(x, nextCol++)) == 1;
					}

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

				const auto t = getTransition();
				t->startTransition(outgoingBlock, 200.0, MT::Rect(outgoingBlock->getX() + (cellWidth * boardWidth) * MT::NumberHelper::random(-1, 1), outgoingBlock->getY(), cellWidth, 0), 0.0);
				activeTransitions.push_back(t);

				outgoingBlock->blockX = -1;
				outgoingBlock->blockY = -1;
			}

			std::list<std::shared_ptr<Block>> blocksToDrop;
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

				if (!child->hasSettled)
				{
					auto isFalling = false, hasEncounteredSettledBlock = false;
					auto childStartingY = child->blockY + 1;
					while (!hasEncounteredSettledBlock && !isFalling && childStartingY <= (int)boardHeight)
					{
						const auto nextMapping = calcMapOffset(child->blockX, childStartingY++);
						isFalling = blockMap.count(nextMapping) == 0;
						hasEncounteredSettledBlock = blockMap.count(nextMapping) == 1 && blockMap[nextMapping]->hasSettled;
					}

					if (isFalling)
					{
						blocksToDrop.push_back(child);
					}
					else
					{
						child->hasSettled = true;
						positionBlock(child);
					}
				}
			}

			auto currentBlocksHaveSettled = false;
			for (const auto currentBlock : currentBlocks)
			{
				currentBlocksHaveSettled = currentBlocksHaveSettled || currentBlock->hasSettled;
			}

			if (currentBlocksHaveSettled)
			{
				for (const auto currentBlock : currentBlocks)
				{
					const auto it = std::find(blocksToDrop.begin(), blocksToDrop.end(), currentBlock);
					if (it != blocksToDrop.end())
					{
						(*it)->hasSettled = true;
						blocksToDrop.erase(it);
					}
				}
				currentBlocks.clear();
			}

			for (const auto blockToDrop : blocksToDrop)
			{
				blockMap.erase(calcMapOffset(blockToDrop));
				blockToDrop->blockY++;
			}

			for (const auto blockToDrop : blocksToDrop)
			{
				blockMap[calcMapOffset(blockToDrop)] = blockToDrop;

				const auto t = modules->animation->createGameTransition();
				t->startTransition(blockToDrop, 50.0, MT::Rect(blockToDrop->blockX * cellWidth, blockToDrop->blockY * cellHeight, cellHeight, cellWidth));
				transitions.push_back(t);
			}
		}
	}
}