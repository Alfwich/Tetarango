#include "Board.h"

namespace
{
	const auto pieceLandSoundName = "tetris-piece-land";
}

namespace AWGame
{
	std::shared_ptr<AW::Transition> Board::createTransition()
	{
		return modules->animation->createTransition();
	}

	void Board::updateBoardIfNeeded()
	{
		if (background != nullptr)
		{
			setScreenSize(boardWidth * cellWidth + cellWidth, boardHeight * cellHeight + cellHeight);
			background->setSizeAndCenter(this, -(cellWidth / 2.0), -(cellHeight / 2.0));
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
			blockPtr->setScreenPosition(blockPtr->blockX * cellWidth, blockPtr->blockY * cellHeight);
		}
	}

	Board::Board()
	{
		GORegister(Board);
	}

	Board::Board(unsigned int boardWidth, unsigned int boardHeight) : Board()
	{
		this->boardWidth = boardWidth;
		this->boardHeight = boardHeight;
		updateBoardIfNeeded();
	}

	void Board::onLoadResources()
	{
		modules->texture->loadTexture("res/image/actor/board/board-background.png", "board-background");
		modules->sound->loadSoundClip("res/sound/piece-land.wav", pieceLandSoundName);
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

	void Board::enableBoardFalling()
	{
		isFallingEnabled = true;
	}

	void Board::disableBoardFalling()
	{
		isFallingEnabled = false;
	}

	bool Board::hasAnActivePiece()
	{
		return currentBlocks.size() > 0;
	}

	bool Board::getHasFailedToPlacePiece()
	{
		return hasFailedToPlacePiece;
	}

	std::vector<std::shared_ptr<Block>> Board::getEliminatedPieces()
	{
		auto result = std::vector<std::shared_ptr<Block>>();
		if (eliminiatedBlocks.size() > 0)
		{
			for (const auto block : eliminiatedBlocks)
			{
				result.push_back(block);
			}
			eliminiatedBlocks.clear();
		}

		return result;
	}

	void Board::resetBoard()
	{
		hasFailedToPlacePiece = false;
		currentBlocks.clear();
		blockMap.clear();
		actionTimer->start();
		for (const auto child : getChildrenOfType<Block>())
		{
			if (child->blockX == -1 || child->blockY == -1)
			{
				continue;
			}

			const auto t = modules->animation->createTransition();
			t->startTransition(child, 200.0, AW::Rect(child->getScreenX() + (cellWidth * boardWidth) * AW::NumberHelper::random(-1, 1), child->getScreenY() + (cellHeight * boardHeight) * AW::NumberHelper::random(-1, 1), 0, 0), 0.0);
			transitions.push_back(t);

			child->blockX = -1;
			child->blockY = -1;
		}
	}

	void Board::add(std::shared_ptr<GameObject> ao)
	{
		const auto blockPtr = std::dynamic_pointer_cast<Block>(ao);
		if (blockPtr != nullptr)
		{
			if (blockPtr->blockX == -1 && blockPtr->blockY == -1)
			{
				return;
			}

			blockPtr->zIndex = 5;

			const auto mapOffset = calcMapOffset(blockPtr);
			if (blockMap.count(mapOffset) != 0)
			{
				hasFailedToPlacePiece = true;
				blockMap.at(mapOffset)->removeFromParent();
				blockMap.erase(mapOffset);
			}

			blockMap[mapOffset] = blockPtr;
			positionBlock(blockPtr);
			Container::add(blockPtr);
		}
		else
		{
			Container::add(ao);
		}
	}

	void Board::addTetromino(const std::vector<std::shared_ptr<Block>>& blocks)
	{
		currentBlocks.clear();
		const auto centerX = boardWidth / 2;
		for (const auto block : blocks)
		{
			block->blockY += 2;
			block->blockX += centerX;
			add(block);
			currentBlocks.push_back(block);
		}

		if (hasFailedToPlacePiece)
		{
			currentBlocks.clear();
		}
	}

	std::vector<std::shared_ptr<Block>> Board::getCurrentBlock()
	{
		auto result = std::vector<std::shared_ptr<Block>>();

		if (currentBlocks.size() == 0)
		{
			return result;
		}

		const auto rotationPoint = currentBlocks[0];
		const auto xOffset = rotationPoint->blockX;
		const auto yOffset = rotationPoint->blockY;

		bool shouldAllowRotation = !hasFailedToPlacePiece;
		for (const auto currentPiece : currentBlocks)
		{
			blockMap.erase(calcMapOffset(currentPiece));
			currentPiece->blockX -= xOffset;
			currentPiece->blockY -= yOffset;
			result.push_back(currentPiece);

			remove(currentPiece);
		}

		currentBlocks.clear();

		return result;
	}

	void Board::moveCurrentPiece(int x, int y)
	{
		auto shouldAllowMove = !hasFailedToPlacePiece;
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

				const auto t = createTransition();
				t->startTransition(currentPiece, 50.0, AW::Rect(currentPiece->blockX * cellWidth, currentPiece->blockY * cellHeight, cellHeight, cellWidth));
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

		bool shouldAllowRotation = !hasFailedToPlacePiece;
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

				const auto t = createTransition();
				t->startTransition(currentPiece, 50.0, AW::Rect(currentPiece->blockX * cellWidth, currentPiece->blockY * cellHeight, cellHeight, cellWidth));
				transitions.push_back(t);
			}
		}
	}

	void Board::onInitialAttach()
	{
		actionTimer = modules->time->createTimer();
		actionTimer->start();
		enableEnterFrame();
	}

	void Board::onCreateChildren()
	{
		background = std::make_shared<AW::NineSlice>();
		background->name = "background";
		updateBoardIfNeeded();
		background->setTexture("board-background");
		background->setCornerSize(16);
		add(background);
	}

	void Board::onChildrenHydrated()
	{
		background = findChildWithName<AW::NineSlice>("background");

		for (const auto child : getChildrenOfType<Block>())
		{
			if (!child->hasSettled && child->blockX != -1 && child->blockY != -1)
			{
				currentBlocks.push_back(child);
			}
		}
	}

	std::shared_ptr<AW::SerializationClient> Board::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__board__", hint);

		boardWidth = (unsigned int)client->serializeInt("board-width", boardWidth);
		boardHeight = (unsigned int)client->serializeInt("board-height", boardHeight);
		cellWidth = (unsigned int)client->serializeInt("cell-width", cellWidth);
		cellHeight = (unsigned int)client->serializeInt("cell-height", cellHeight);
		isFallingEnabled = client->serializeBool("board-is-falling-enabled", isFallingEnabled);
		hasFailedToPlacePiece = client->serializeBool("board-has-failed-to-place-piece", hasFailedToPlacePiece);
		energyShiftEnabled = client->serializeBool("board-energy-shift-enabled", energyShiftEnabled);

		return Container::doSerialize(hint);
	}

	void Board::onEnterFrame(const double& frameTime)
	{
		if (energyShiftEnabled)
		{
			for (unsigned int x = 0; x <= boardWidth; ++x)
			{
				for (unsigned int y = 0; y <= boardHeight; ++y)
				{
					const auto mapping = calcMapOffset(x, y);
					if (blockMap.count(mapping) == 0)
					{
						continue;
					}

					const auto blockPtr = blockMap.at(mapping);
					if (blockPtr->hasSettled && blockPtr->getEnergy() > 0.0)
					{
						const auto nextMapping = calcMapOffset(blockPtr->blockX, blockPtr->blockY + 1);
						if (blockMap.count(nextMapping) == 1)
						{
							const auto nextBlockPtr = blockMap.at(nextMapping);
							if (nextBlockPtr->hasSettled && nextBlockPtr->getEnergy() < 1.0)
							{
								const auto r = nextBlockPtr->addEnergy(blockPtr->removeEnergy((frameTime / 1000.0)));
								blockPtr->addEnergy(r);
							}
						}
					}

				}
			}

			for (unsigned int x = 0; x <= boardWidth; ++x)
			{
				for (unsigned int y = 0; y <= boardHeight; ++y)
				{
					const auto mapping = calcMapOffset(x, y);
					if (blockMap.count(mapping) == 0)
					{
						continue;
					}

					const auto blockPtr = blockMap.at(mapping);
					if (blockPtr->hasSettled && blockPtr->getEnergy() > 0.0)
					{
						const auto leftNeightbor = calcMapOffset(blockPtr->blockX - 1, blockPtr->blockY);
						if (blockMap.count(leftNeightbor) == 1)
						{
							const auto leftBlockPtr = blockMap.at(leftNeightbor);
							if (leftBlockPtr->hasSettled && leftBlockPtr->getEnergy() < blockPtr->getEnergy())
							{
								const auto r = leftBlockPtr->addEnergy(blockPtr->removeEnergy((frameTime / 1000.0)));
								blockPtr->addEnergy(r);
							}
						}

						const auto rightNeightbor = calcMapOffset(blockPtr->blockX + 1, blockPtr->blockY);
						if (blockMap.count(rightNeightbor) == 1)
						{
							const auto rightBlockPtr = blockMap.at(rightNeightbor);
							if (rightBlockPtr->hasSettled && rightBlockPtr->getEnergy() < blockPtr->getEnergy())
							{
								const auto r = rightBlockPtr->addEnergy(blockPtr->removeEnergy((frameTime / 1000.0)));
								blockPtr->addEnergy(r);
							}
						}
					}

				}
			}
		}

		if (isFallingEnabled && !hasFailedToPlacePiece && actionTimer->isAboveThresholdAndRestart(isFastFalling ? 25 : 750))
		{
			for (auto it = transitions.begin(); it != transitions.end();)
			{
				const auto transition = (*it);
				if (!transition->isPlaying())
				{
					it = transitions.erase(it);
				}
				else
				{
					++it;
				}
			}

			std::vector<std::shared_ptr<Block>> outgoingBlocks;
			int lowestRowToOutgo = -1;
			unsigned int numRowsToDrop = 0;
			for (int y = 0; y <= (int)boardHeight; ++y)
			{
				auto x = 0;
				auto rowFilled = true;

				while (rowFilled && x <= (int)boardWidth)
				{
					const auto pos = calcMapOffset(x++, y);
					rowFilled = blockMap.count(pos) == 1 && blockMap[pos]->hasSettled;
				}

				if (rowFilled)
				{
					if (y > lowestRowToOutgo)
					{
						lowestRowToOutgo = y;
					}

					numRowsToDrop++;

					for (unsigned int x = 0; x <= boardWidth; ++x)
					{
						outgoingBlocks.push_back(blockMap[calcMapOffset(x, y)]);
					}
				}
			}

			for (const auto outgoingBlock : outgoingBlocks)
			{
				blockMap.erase(calcMapOffset(outgoingBlock->blockX, outgoingBlock->blockY));

				const auto t = modules->animation->createTransition();
				t->startTransition(outgoingBlock, 200.0, AW::Rect(outgoingBlock->getScreenX() + (cellWidth * boardWidth) * AW::NumberHelper::random(-1, 1), outgoingBlock->getScreenY(), cellWidth, 0), 0.0);
				transitions.push_back(t);

				outgoingBlock->blockX = -1;
				outgoingBlock->blockY = -1;

				eliminiatedBlocks.push_back(outgoingBlock);
			}

			std::list<std::shared_ptr<Block>> blocksToDrop;
			if (lowestRowToOutgo != -1)
			{
				for (const auto child : getChildrenOfType<Block>())
				{
					if (child->blockX == -1 || child->blockY == -1)
					{
						continue;
					}

					if (child->hasSettled && child->blockY < lowestRowToOutgo)
					{
						blocksToDrop.push_back(child);
					}
				}
			}

			for (const auto blockToDrop : blocksToDrop)
			{
				blockMap.erase(calcMapOffset(blockToDrop));
				blockToDrop->blockY += numRowsToDrop;
			}

			for (const auto blockToDrop : blocksToDrop)
			{
				blockMap[calcMapOffset(blockToDrop)] = blockToDrop;

				const auto t = createTransition();
				t->startTransition(blockToDrop, 50.0, AW::Rect(blockToDrop->blockX * cellWidth, blockToDrop->blockY * cellHeight, cellHeight, cellWidth));
				transitions.push_back(t);
			}

			blocksToDrop.clear();
			for (const auto child : getChildrenOfType<Block>())
			{
				if (child->blockX == -1 || child->blockY == -1)
				{
					if (child->getScreenWidth() == 0 || child->getScreenHeight() == 0)
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
				modules->sound->playSoundClip(pieceLandSoundName);
			}

			for (const auto blockToDrop : blocksToDrop)
			{
				blockMap.erase(calcMapOffset(blockToDrop));
				blockToDrop->blockY++;
			}

			for (const auto blockToDrop : blocksToDrop)
			{
				blockMap[calcMapOffset(blockToDrop)] = blockToDrop;

				const auto t = createTransition();
				t->startTransition(blockToDrop, 50.0, AW::Rect(blockToDrop->blockX * cellWidth, blockToDrop->blockY * cellHeight, cellHeight, cellWidth));
				transitions.push_back(t);
			}
		}
	}
}