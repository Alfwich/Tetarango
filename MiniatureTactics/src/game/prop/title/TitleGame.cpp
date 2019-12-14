#include "TitleGame.h"

#include "ui/renderable/element/Rectangle.h"

namespace
{
	const auto fontSizeParam = "t-g-f-s";
	const auto numBackgroundTitles = 3;
	const auto titleMoveFactor = 2.0;
	std::vector<MT::Color> colors
	{
		MT::Color::white(),
		MT::Color(64, 64, 64)
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

	void TitleGame::onCreateChildren()
	{
		primaryTitle = std::make_shared<MT::Text>();
		primaryTitle->setFont("medium", getFontSize());
		primaryTitle->setText(modules->gameConfig->getConfigString(Config::Param::gameName));
		primaryTitle->setColor(MT::Color::white());
		primaryTitle->centerAlignSelf();
		add(primaryTitle);

		titles.clear();

		auto zIndex = -1;
		for (auto i = 0; i < numBackgroundTitles; ++i)
		{
			const auto newTitle = std::make_shared<MT::Element>();
			newTitle->setAlpha(0.75);
			newTitle->setColor(colors[i % colors.size()]);
			newTitle->setMatchSizeToTexture(true);
			newTitle->setTexture(primaryTitle->getTextureText());
			newTitle->zIndex = MT::NumberHelper::clamp<int>(zIndex--, -20, 0);

			titles.push_back(newTitle);
			add(newTitle);
		}
	}

	void TitleGame::onLayoutChildren()
	{
		for (auto i = 0; i < numBackgroundTitles; ++i)
		{
			const auto title = titles[i];
			const auto cl = colors[0].lerp(colors[1], i / (double)numBackgroundTitles);
			title->setSizeAndPosition(MT::Rect(primaryTitle->getX() + i * titleMoveFactor, primaryTitle->getY() + i * titleMoveFactor, title->getWidth(), title->getHeight()));
			title->setColor(cl);
		}
	}
}
