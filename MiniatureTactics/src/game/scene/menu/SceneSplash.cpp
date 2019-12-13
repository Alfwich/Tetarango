#include "SceneSplash.h"
#include <iostream>

#include "ui/renderable/element/Rectangle.h"
#include "generator/block/GeneratorBlock.h"

namespace
{
	const auto sdlLogoTextureName = "splash-sdl-logo";
	const auto openGLLogoTextureName = "splash-opengl-logo";
	const auto loadingPatternTextureName = "splash-loading-pattern";
	const auto verticalOffset = -128.0;
	const auto splashTransitionTimeInSeconds = 16.5;
	const auto fadeInHorMovement = 250.0;
	const auto animationScalingFactorIn = 0.25;
	const auto animationScalingFactorOut = 1 / animationScalingFactorIn;
	const auto animationTiming = 1.0;
	const auto titleFontSizeBig = 150;
	const auto titleFontSizeSmall = 75;
	const auto loadingBarHeight = 5.0;
	const auto numBlocksToMake = 200;
	const auto blockHeightGenerationLimit = -6000.0;
	const auto titleBackgroundMovement = 9.0;

	const std::vector<MT::Color> colors
	{
		MT::Color(0xa001efff),
		MT::Color(0x0001f2ff),
		MT::Color(0xf00100ff),
		MT::Color(0xf0f001ff),
		MT::Color(0x00f000ff),
		MT::Color(0xefa000ff),
		MT::Color(0x01f0f1ff),
		MT::Color(0xa001efff),
		MT::Color(0x0001f2ff),
		MT::Color(0xf00100ff),
		MT::Color(0xf0f001ff),
		MT::Color(0x00f000ff),
		MT::Color(0xefa000ff),
		MT::Color(0x01f0f1ff)

	};

	MTGame::GeneratorBlock blockColorGenerator;
}

namespace MTGame
{

	SceneSplash::SceneSplash() : BaseScene(SceneGame::Splash)
	{
		rebuildOnLoad = true;
		enableSerialization<SceneSplash>();
	}

	void SceneSplash::onLoadResources()
	{
		modules->texture->loadTexture("res/game/img/splash/sdl-logo.png", sdlLogoTextureName);
		modules->texture->loadTexture("res/game/img/splash/opengl-logo.png", openGLLogoTextureName);
		modules->texture->loadTexture("res/game/img/splash/loading-pattern.png", loadingPatternTextureName);
	}

	void SceneSplash::onCreateChildren()
	{
		blockContainer = std::make_shared<MT::Container>();
		blockContainer->setColor(88, 88, 88);
		blockContainer->setExpandToChildren(false);
		add(blockContainer);

		splashText = std::make_shared<MT::Text>();
		splashText->setFont("medium", titleFontSizeBig);
		splashText->setText("built with");
		add(splashText);

		splashImage = std::make_shared<MT::Element>();
		splashImage->setTexture(sdlLogoTextureName);
		splashImage->setMatchSizeToTexture(true);
		add(splashImage);

		loadingProgressBar = std::make_shared<MT::Element>();
		loadingProgressBar->setTexture(loadingPatternTextureName);
		add(loadingProgressBar);
	}

	void SceneSplash::onInitialAttach()
	{
		splashTransition = modules->animation->createTransitionForTimeScope(MT::TimeScope::Menu);
		splashTransition->listener = std::dynamic_pointer_cast<MT::INotifyOnTransition>(shared_from_this());

		splashTitleMoveTimer = modules->time->createTimer();

		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, baseSceneWeakThisRef());
		modules->input->keyboard->registerKey(SDL_SCANCODE_SPACE, baseSceneWeakThisRef());
	}

	void SceneSplash::onAttach()
	{
		state = 0;
		blocks.clear();
		splashText->setAlpha(0.0);
		splashImage->setAlpha(0.0);
		loadingProgressBar->setColor(150, 150, 150);
		splashTransition->startTargetlessTransition(splashTransitionTimeInSeconds * 1000.0);
	}

	void SceneSplash::onTransitionAnimationFrame(double position)
	{
		const auto currentTransitionSeconds = position * splashTransitionTimeInSeconds;
		const auto scaledMainPositionIn = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorIn), 0.0, 1.0);
		const auto scaledMainPositionOut = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorOut), 0.0, 1.0);
		const auto scaledSecondaryPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorIn), 0.0, 2.0);

		loadingProgressBar->setWidth(getScreenWidth() * position);
		if (currentTransitionSeconds <= splashTransitionTimeInSeconds - 1.0)
		{
			loadingProgressBar->setHeight(loadingBarHeight * MT::NumberHelper::clamp<double>(currentTransitionSeconds, 0.0, 1.0));
		}
		else
		{
			const auto p = currentTransitionSeconds - (splashTransitionTimeInSeconds - 1);
			loadingProgressBar->setHeight(loadingBarHeight * MT::NumberHelper::clamp<double>(1 - p, 0.0, 1.0));
		}

		loadingProgressBar->centerAlignSelf(0.0, getScreenHeight() - loadingProgressBar->getHeight());

		if (state == 0)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashText->toTopOf(splashImage);

			splashText->setAlpha(scaledMainPositionIn);
			splashImage->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 1)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashImage->setTexture(openGLLogoTextureName);
			}
		}
		else if (state == 2)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashImage->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 3)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);
			splashText->toTopOf(splashImage);
			splashText->setAlpha(1.0 - scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashText->setFontSize(titleFontSizeBig);
				const auto orgName = modules->gameConfig->getConfigString(Config::Param::organizationName);
				splashText->setText(orgName + " Presents");
				if (splashText->getWidth() > getScreenWidth())
				{
					splashText->setFontSize(titleFontSizeSmall);
				}
			}
		}
		else if (state == 4)
		{
			splashText->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0);
			splashText->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 5)
		{
			splashText->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getScreenHeight() / 2.0);
			splashText->setAlpha(1.0 - scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashText->setFontSize(titleFontSizeBig);
				splashText->setText(modules->gameConfig->getConfigString(Config::Param::gameName));
				if (splashText->getWidth() > getScreenWidth())
				{
					splashText->setFontSize(titleFontSizeSmall);
				}

				const auto titleRotationOffsetBase = (MT::NumberHelper::PI * 2.0) / colors.size();
				auto rotationOffset = 0.0;
				auto zIndex = -1;
				for (const auto c : colors)
				{
					const auto newTitle = std::make_shared<MT::Element>();
					newTitle->setColor(c);
					newTitle->setMatchSizeToTexture(true);
					newTitle->setTexture(splashText->getTextureText());
					newTitle->setPosition(getScreenWidth() / 2.0 - ((1.0 - 0.0) * fadeInHorMovement), getScreenHeight() / 2.0);
					newTitle->zIndex = zIndex--;
					newTitle->setAlpha(0.0);
					titles.push_back(newTitle);
					titleOffsets.push_back(std::make_pair(0.0, 0.0));
					titleRotationOffsets.push_back(rotationOffset);
					titleTransitions.push_back(modules->animation->createTransition());
					add(newTitle);

					rotationOffset += titleRotationOffsetBase;
				}

				for (auto i = 0; i < numBlocksToMake; ++i)
				{
					for (auto block : blockColorGenerator.getTetromino())
					{
						block->setScale(0.5);
						block->setRotation(MT::NumberHelper::random(0.0, 360.0));
						block->setX(MT::NumberHelper::random() * getScreenWidth());
						block->setY(MT::NumberHelper::random(-100.0, blockHeightGenerationLimit));
						block->zIndex = zIndex;
						blocks.push_back(block);
						blockContainer->add(block);
					}
				}

				splashTitleMoveTimer->start();
				enableEnterFrame(-1);
			}
		}
		else if (state == 6)
		{
			splashText->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0);
			splashText->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 6.0);
		}
		else if (state == 7)
		{
			splashText->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * 0.0), getScreenHeight() / 2.0);
			splashText->setAlpha(1.0 - scaledMainPositionOut);
		}
	}

	void SceneSplash::onEnterFrame(double frameTime)
	{
		for (const auto block : blocks)
		{
			block->movePosition(0.0, 800.0 * (frameTime / 1000.0));
			block->rotate(90.0 * (frameTime / 1000.0));

			if (state == 7)
			{
				block->setScale(MT::NumberHelper::clamp<double>(block->getScaleX() - frameTime / 1000.0, 0.0, 1.0));
			}
		}

		for (auto i = 0; i < titles.size(); ++i)
		{
			const auto title = titles[i];
			const auto titleOffset = titleOffsets[i];

			title->setSizeAndPosition(MT::Rect(splashText->getX() + titleOffset.first, splashText->getY() + titleOffset.second, title->getWidth(), title->getHeight()));
			title->setAlpha(splashText->getAlpha() - 0.45);
		}

		for (auto i = 0; i < titleOffsets.size(); ++i)
		{
			const auto titleRotationOffset = titleRotationOffsets[i];
			titleOffsets[i] = std::make_pair(std::cos(titleRotationOffset + (accFrameTime / 1000.0)) * titleBackgroundMovement, std::sin(titleRotationOffset + (accFrameTime / 1000.0)) * titleBackgroundMovement);
		}

		accFrameTime += frameTime * MT::NumberHelper::PI * 2.0;
	}

	bool SceneSplash::tryToGotoNextState(double position, double timeThreshold)
	{
		const auto currentTransitionSeconds = position * splashTransitionTimeInSeconds;
		const auto currentStateTime = currentTransitionSeconds - animationOffset * splashTransitionTimeInSeconds;
		const auto isAboveThreshold = currentStateTime > timeThreshold;

		if (isAboveThreshold)
		{
			animationOffset = position;
			state++;
		}

		return isAboveThreshold;
	}

	void SceneSplash::onTransitionCompleted()
	{
		for (const auto block : blocks)
		{
			block->removeFromParent();
		}
		blocks.clear();

		for (const auto title : titles)
		{
			title->removeFromParent();
		}
		titles.clear();
		titleOffsets.clear();
		titleTransitions.clear();

		disableEnterFrame();

		destroyChildren();

		transitionToScene(SceneGame::MainMenu);
	}

	void SceneSplash::onMouseButtonLeftDown()
	{
		splashTransition->finish();
	}

	void SceneSplash::onKeyPressed(SDL_Scancode key)
	{
		splashTransition->finish();
	}

}
