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
		blockContainer->zIndex = -10;
		add(blockContainer);

		splashText = std::make_shared<MT::Text>();
		splashText->setFont("medium", titleFontSizeBig);
		splashText->setText("built with");
		add(splashText);

		splashImage = std::make_shared<MT::Element>();
		splashImage->setTexture(sdlLogoTextureName);
		splashImage->setMatchSizeToTexture(true);
		add(splashImage);

		titleGame = std::make_shared<TitleGame>();
		titleGame->setFontSize(150);
		add(titleGame);

		loadingProgressBar = std::make_shared<MT::Element>();
		loadingProgressBar->setTexture(loadingPatternTextureName);
		add(loadingProgressBar);
	}

	void SceneSplash::onInitialAttach()
	{
		splashTransition = modules->animation->createTransitionForTimeScope(MT::TimeScope::Menu);
		splashTransition->listener = weak_from_this();

		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, baseSceneWeakThisRef());
		modules->input->keyboard->registerKey(SDL_SCANCODE_SPACE, baseSceneWeakThisRef());
	}

	void SceneSplash::onAttach()
	{
		state = 0;
		blocks.clear();
		splashText->setAlpha(0.0);
		splashImage->setAlpha(0.0);
		titleGame->setAlpha(0.0);
		loadingProgressBar->setColor(150, 150, 150);
		loadingProgressBar->setHeight(loadingBarHeight);
		splashTransition->startTargetlessTransition(splashTransitionTimeInSeconds * 1000.0);
	}

	void SceneSplash::onTransitionAnimationFrame(double position)
	{
		const auto currentTransitionSeconds = position * splashTransitionTimeInSeconds;
		const auto scaledMainPositionIn = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorIn), 0.0, 1.0);
		const auto scaledMainPositionOut = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorOut), 0.0, 1.0);

		loadingProgressBar->setWidth(getScreenWidth() * position);
		loadingProgressBar->centerAlignSelf(0.0, getScreenHeight() - loadingProgressBar->getHeight());

		if (currentTransitionSeconds >= splashTransitionTimeInSeconds - 1.0)
		{
			const auto p = currentTransitionSeconds - (splashTransitionTimeInSeconds - 1.0);
			loadingProgressBar->setAlpha(MT::NumberHelper::clamp<double>(1.0 - p, 0.0, 1.0));
		}

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
				for (auto i = 0; i < numBlocksToMake; ++i)
				{
					for (auto block : blockColorGenerator.getTetromino())
					{
						const auto randomX = MT::NumberHelper::random(blockHeightGenerationLimit, -100);
						const auto randomY = MT::NumberHelper::random(blockHeightGenerationLimit, -100);
						block->setScale(MT::NumberHelper::random(0.25, 0.75));
						block->setX(randomX);
						block->setY(randomY);
						blocks.push_back(block);
						blockContainer->add(block);
					}
				}

				titleGame->visible = true;
				enableEnterFrame();
			}
		}
		else if (state == 6)
		{
			titleGame->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0);
			titleGame->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 6.0);
		}
		else if (state == 7)
		{
			titleGame->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * 0.0), getScreenHeight() / 2.0);
			titleGame->setAlpha(1.0 - scaledMainPositionOut);
		}
	}

	void SceneSplash::onEnterFrame(double frameTime)
	{
		for (const auto block : blocks)
		{
			const auto deltaTime = frameTime / 1000.0;
			block->movePosition(600.0 * deltaTime, 600.0 * deltaTime);

			if (state == 7)
			{
				block->setScale(MT::NumberHelper::clamp<double>(block->getScaleX() - frameTime / 1000.0, 0.0, 1.0));
			}
		}
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
