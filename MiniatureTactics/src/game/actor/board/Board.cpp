#include "Board.h"
#include "ui/renderable/element/Rectangle.h"

namespace MTGame
{

	bool Board::isLoaded = false;

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

	void Board::onInitialAttach()
	{
		enableEnterFrame();
	}

	void Board::onCreateChildren()
	{
		auto rect = std::make_shared<MT::Rectangle>();
		rect->setSize(500, 500);
		rect->setColor(255, 255, 255);
		add(rect);
	}

	void Board::onEnterFrame(double deltaTime)
	{
		rotate(45 * deltaTime / 1000);
	}
}