#include "TitleGame.h"

#include "ui/renderable/primitive/Rectangle.h"

namespace
{
	const auto fontSizeParam = "t-g-f-s";
	std::vector<AW::Color> colors
	{
		AW::Color::white(),
		AW::Color(64, 64, 64)
	};
}

namespace AWGame
{
	TitleGame::TitleGame()
	{
		rebuildOnLoad = true;
		registerSerialization<TitleGame>();
	}

	void TitleGame::setFontSize(int fontSize)
	{
		serializationClient->setInt(fontSizeParam, fontSize);
	}

	int TitleGame::getFontSize()
	{
		return serializationClient->getInt(fontSizeParam, 75);
	}

	void TitleGame::onInitialAttach()
	{
		introTransition = modules->animation->createTransition();
		introTransition->listener = weak_from_this();
		introTransition->startTargetlessTransition(2000.0);

		continuousTransition = modules->animation->createTransition();
		continuousTransition->listener = weak_from_this();
	}

	void TitleGame::onCreateChildren()
	{
		primaryTitle = std::make_shared<AW::Text>();
		primaryTitle->setFont("medium", getFontSize());
		primaryTitle->setText(modules->gameConfig->getConfigString(Config::Param::gameName));
		primaryTitle->setColor(AW::Color::white());
		primaryTitle->visible = false;
		add(primaryTitle);

		titles.clear();

		auto zIndex = -1;
		for (const auto c : primaryTitle->getText())
		{
			const auto newTitle = std::make_shared<AW::Text>();
			newTitle->setFont("medium", getFontSize());
			newTitle->setAlpha(0.0);
			newTitle->setColor(colors[0]);
			newTitle->setText(std::string(1, c));
			newTitle->zIndex = AW::NumberHelper::clamp<int>(zIndex--, -20, 0);

			titles.push_back(newTitle);
			add(newTitle);
		}

		std::reverse(titles.begin(), titles.end());
	}

	void TitleGame::onLayoutChildren()
	{
		std::shared_ptr<AW::Text> lastTitle;
		for (const auto title : titles)
		{
			if (lastTitle != nullptr)
			{
				title->toLeftOf(lastTitle);
			}
			else
			{
				title->setPosition(primaryTitle->getX() + primaryTitle->getHalfWidth() - title->getHalfWidth(), primaryTitle->getY());
			}

			lastTitle = title;
		}

		resizeSelfToChildrenAndCenterChildren();
	}

	void TitleGame::onTimeoutCalled()
	{
		if (isAttached())
		{
			continuousTransition->startTargetlessTransition(1000.0);
		}
	}

	void TitleGame::onTransitionAnimationFrame(double position, int id)
	{
		for (const auto title : titles)
		{
			if (id == introTransition->getId())
			{
				const auto p = AW::NumberHelper::clamp(std::pow(position, 1.0 / 5.0), 0.0, 1.0);
				title->setAlpha(p);
				const auto offset = std::sin((title->getX() / (getWidth() / 2.0)) + p * AW::NumberHelper::PI * 2.0) * 200.0;
				title->setY(primaryTitle->getY() + offset * (1.0 - p));
			}
			else if (id == continuousTransition->getId())
			{
				if (position < 1.0)
				{
					const auto colorI = ((int)std::floor(position * colors.size())) % colors.size();
					const auto nextColorI = (colorI + 1) % colors.size();
					const auto colorP = (position * colors.size()) - colorI;
					title->setColor(colors[colorI].lerp(colors[nextColorI], colorP));
				}
				else
				{
					title->setColor(colors[0]);
				}
				const auto period = title->getX() / (getWidth() / 16.0);
				if (position < 0.5)
				{
					const auto p = position * 2.0;
					const auto offset = std::sin(period + p * AW::NumberHelper::PI * 2.0) * 10.0;
					title->setY(primaryTitle->getY() - std::abs(offset * p));
				}
				else
				{
					const auto p = (position - 0.5) * 2.0;
					const auto offset = std::sin(period + p * AW::NumberHelper::PI * 2.0) * 10.0;
					title->setY(primaryTitle->getY() - std::abs(offset * (1.0 - p)));
				}
			}
		}
	}

	void TitleGame::onTransitionCompleted()
	{
		setTimeout(2000.0);
	}
}
