#include "TitleGame.h"

#include "ui/renderable/element/Rectangle.h"

namespace
{
	const auto fontSizeParam = "t-g-f-s";
	const auto numBackgroundTitles = 14;
	const auto backgroundTitleMovementAmount = 6.0;
	const std::vector<MT::Color> colors
	{
		MT::Color(0xa001efff),
		MT::Color(0x0001f2ff),
		MT::Color(0xf00100ff),
		MT::Color(0xf0f001ff),
		MT::Color(0x00f000ff),
		MT::Color(0xefa000ff),
		MT::Color(0x01f0f1ff)
	};

}

namespace MTGame
{
	TitleGame::TitleGame()
	{
		rebuildOnLoad = true;
		enableSerialization<TitleGame>();
	}

	void TitleGame::setFontSize(int fontSize)
	{
		serializationClient->setInt(fontSizeParam, MT::NumberHelper::clamp<int>(fontSize, 0, 1000));
	}

	int TitleGame::getFontSize()
	{
		return serializationClient->getInt(fontSizeParam, 75);
	}

	void TitleGame::onInitialAttach()
	{
		enableEnterFrame();
	}

	void TitleGame::onCreateChildren()
	{
		primaryTitle = std::make_shared<MT::Text>();
		primaryTitle->setFont("medium", getFontSize());
		primaryTitle->setText(modules->gameConfig->getConfigString(Config::Param::gameName));
		primaryTitle->centerAlignSelf();
		add(primaryTitle);

		const auto offsetAmount = (MT::NumberHelper::PI * 2.0) / numBackgroundTitles;
		titles.clear();
		rotationOffset.clear();

		auto zIndex = -1;
		auto currentRotationOffset = 0.0;
		for (auto i = 0; i < numBackgroundTitles; ++i)
		{
			const auto newTitle = std::make_shared<MT::Element>();
			newTitle->setAlpha(0.35);
			newTitle->setColor(colors[i % colors.size()]);
			newTitle->setMatchSizeToTexture(true);
			newTitle->setTexture(primaryTitle->getTextureText());
			newTitle->zIndex = MT::NumberHelper::clamp<int>(zIndex--, -20, 0);

			titles.push_back(newTitle);
			rotationOffset.push_back(currentRotationOffset);
			currentRotationOffset += offsetAmount;

			add(newTitle);
		}
	}

	void TitleGame::onLayoutChildren()
	{
		for (auto i = 0; i < numBackgroundTitles; ++i)
		{
			const auto title = titles[i];
			const auto rotOffset = rotationOffset[i];
			const auto xOffset = std::cos(currentRuntime + rotOffset) * backgroundTitleMovementAmount;
			const auto yOffset = std::sin(currentRuntime + rotOffset) * backgroundTitleMovementAmount;
			title->setSizeAndPosition(MT::Rect(primaryTitle->getX() + xOffset, primaryTitle->getY() + yOffset, title->getWidth(), title->getHeight()));
		}
	}

	void TitleGame::onEnterFrame(double frameTime)
	{
		currentRuntime += (frameTime / 1000.0) * (MT::NumberHelper::PI * 2.0);
		layout();
	}
}
