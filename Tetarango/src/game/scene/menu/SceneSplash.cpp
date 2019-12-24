#include "SceneSplash.h"
#include <iostream>

#include "ui/renderable/primitive/Rectangle.h"
#include "generator/block/GeneratorBlock.h"

namespace
{
	const auto sdlLogoTextureName = "splash-sdl-logo";
	const auto openGLLogoTextureName = "splash-opengl-logo";
	const auto loadingPattenTextureName = "splash-loading-pattern";
	const auto awGamesLogoTextureName = "splash-aw-games-logo";
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
	const auto blurAmount = 1.0;

	AWGame::GeneratorBlock blockColorGenerator;
}

namespace AWGame
{

	SceneSplash::SceneSplash() : BaseScene(SceneGame::Splash)
	{
		setShouldRebuildOnLoad();
		registerGameObject<SceneSplash>();
	}

	void SceneSplash::onLoadResources()
	{
		modules->texture->loadTexture("res/image/splash/sdl-logo.png", sdlLogoTextureName);
		modules->texture->loadTexture("res/image/splash/opengl-logo.png", openGLLogoTextureName);
		modules->texture->loadTexture("res/image/splash/loading-pattern.png", loadingPattenTextureName);
		modules->texture->loadTexture("res/image/splash/aw-games-logo.png", awGamesLogoTextureName);
	}

	void SceneSplash::onCreateChildren()
	{
		blockContainer = std::make_shared<AW::Container>();
		blockContainer->setColor(225, 225, 225);
		blockContainer->zIndex = -1;
		add(blockContainer);

		splashImage = std::make_shared<AW::Element>();
		splashImage->setFragmentShader(modules->shader->getShader({ "f-step-texture", "element" }));
		splashImage->setTexture(sdlLogoTextureName);
		splashImage->setMatchSizeToTexture(true);
		add(splashImage);

		splashText = std::make_shared<AW::Text>();
		splashText->setFragmentShader(modules->shader->getShader({ "f-step-texture", "element" }));
		splashText->setFont("medium", titleFontSizeBig);
		splashText->setText("built with");
		add(splashText);

		titleGame = std::make_shared<TitleGame>();
		titleGame->setFontSize(150);
		add(titleGame);

		loadingProgressBar = std::make_shared<AW::Element>();
		loadingProgressBar->setTexture(loadingPattenTextureName);
		add(loadingProgressBar);
	}

	void SceneSplash::onInitialAttach()
	{
		splashTransition = modules->animation->createTransitionForTimeScope(AW::TimeScope::Menu);
		splashTransition->listener = weak_from_this();

		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, baseSceneWeakThisRef());
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
		const auto scaledMainPositionIn = AW::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorIn), 0.0, 1.0);
		const auto scaledMainPositionOut = AW::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorOut), 0.0, 1.0);

		loadingProgressBar->setWidth(getScreenWidth() * position);
		loadingProgressBar->topLeftAlignSelf(0.0, getScreenHeight() - loadingProgressBar->getHeight());

		if (currentTransitionSeconds >= splashTransitionTimeInSeconds - 1.0)
		{
			const auto p = currentTransitionSeconds - (splashTransitionTimeInSeconds - 1.0);
			loadingProgressBar->setAlpha(AW::NumberHelper::clamp<double>(1.0 - p, 0.0, 1.0));
		}

		if (state == 0)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashImage->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * (1.0 - scaledMainPositionIn));
			splashImage->setAlpha(scaledMainPositionIn);

			splashText->toTopOf(splashImage);
			splashText->setAlpha(scaledMainPositionIn);
			splashText->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * (1.0 - scaledMainPositionIn));

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 1)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashImage->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * scaledMainPositionOut);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashImage->setTexture(openGLLogoTextureName);
			}
		}
		else if (state == 2)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashImage->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * (1.0 - scaledMainPositionIn));
			splashImage->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 3)
		{
			splashImage->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight() + verticalOffset);
			splashImage->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * scaledMainPositionOut);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);

			splashText->toTopOf(splashImage);
			splashText->setAlpha(1.0 - scaledMainPositionOut);
			splashText->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashText->setFontSize(titleFontSizeBig);
				const auto orgName = modules->gameConfig->getConfigString(Config::Param::organizationName);
				splashText->setText(orgName + " Presents");
				splashImage->setTexture(awGamesLogoTextureName);
				if (splashText->getWidth() > getScreenWidth())
				{
					splashText->setFontSize(titleFontSizeSmall);
				}
			}
		}
		else if (state == 4)
		{
			splashText->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getScreenHeight() / 2.0 + splashImage->getHalfHeight());
			splashText->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * (1.0 - scaledMainPositionIn));
			splashText->setAlpha(scaledMainPositionIn);

			splashImage->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * (1.0 - scaledMainPositionIn));
			splashImage->toTopOf(splashText);
			splashImage->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 5)
		{
			splashText->setAlpha(1.0 - scaledMainPositionOut);
			splashText->setPosition(getScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getScreenHeight() / 2.0 + splashImage->getHalfHeight());
			splashText->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * scaledMainPositionOut);

			splashImage->toTopOf(splashText);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);
			splashImage->getFragmentShader()->setFloatIUParam("blurAmount", blurAmount * scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashText->getFragmentShader()->setFloatIUParam("blurAmount", 0.0);
				splashImage->getFragmentShader()->setFloatIUParam("blurAmount", 0.0);
				for (auto i = 0; i < numBlocksToMake; ++i)
				{
					for (auto block : blockColorGenerator.getTetromino())
					{
						const auto randomX = AW::NumberHelper::random(blockHeightGenerationLimit, -100);
						const auto randomY = AW::NumberHelper::random(blockHeightGenerationLimit, -100);
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

	void SceneSplash::onEnterFrame(const double& frameTime)
	{
		const auto deltaTime = frameTime / 1000.0;
		const auto movePos = 600.0 * deltaTime;
		for (const auto block : blocks)
		{
			block->movePosition(movePos, movePos);

			if (state == 7)
			{
				block->setScale(AW::NumberHelper::clamp<double>(block->getScaleX() - deltaTime, 0.0, 1.0));
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
