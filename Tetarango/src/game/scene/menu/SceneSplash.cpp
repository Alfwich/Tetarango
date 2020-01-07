#include "SceneSplash.h"

#include "ui/renderable/primitive/Rectangle.h"
#include "generator/block/GeneratorBlock.h"

namespace
{
	const auto sdlLogoTextureName = "splash-sdl-logo";
	const auto openGLLogoTextureName = "splash-opengl-logo";
	const auto box2dLogoTextureName = "splash-box2d-logo";
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
	const auto scanlineAmount = 20.0;
	const auto xScaleAmount = 0.35;

	AWGame::GeneratorBlock blockColorGenerator;
}

namespace AWGame
{

	SceneSplash::SceneSplash() : BaseScene(SceneGame::Splash)
	{
		setShouldRebuildOnLoad();
		GORegister(SceneSplash);
	}

	void SceneSplash::onLoadResources()
	{
		modules->texture->loadTexture("res/image/splash/sdl-logo.png", sdlLogoTextureName);
		modules->texture->loadTexture("res/image/splash/opengl-logo.png", openGLLogoTextureName);
		modules->texture->loadTexture("res/image/splash/box2d-logo.png", box2dLogoTextureName);
		modules->texture->loadTexture("res/image/splash/loading-pattern.png", loadingPattenTextureName);
		modules->texture->loadTexture("res/image/splash/aw-games-logo.png", awGamesLogoTextureName);
	}

	void SceneSplash::onCreateChildren()
	{
		splashImage = std::make_shared<AW::Element>();
		splashImage->setFragmentShader(modules->shader->getShader({ "element", "f-scanline-retro" }));
		splashImage->getFragmentShader()->setFloatIUParam("fScanlineRetroAmount", 1.0);
		splashImage->setTexture(sdlLogoTextureName);
		splashImage->setMatchSizeToTexture(true);
		add(splashImage);

		splashText = std::make_shared<AW::Text>();
		splashText->setFragmentShader(modules->shader->getShader({ "element" }));
		splashText->setFont("medium", titleFontSizeBig);
		splashText->setText("built with");
		add(splashText);

		titleGame = std::make_shared<TitleGame>();
		titleGame->setFontSize(150);
		add(titleGame);

		titleGameCollider = std::make_shared<AW::Box>();
		titleGameCollider->layoutSpace = AW::LayoutSpace::Screen;
		titleGameCollider->visible = false;
		titleGameCollider->setDynamic(false);

		loadingProgressBar = std::make_shared<AW::Element>();
		loadingProgressBar->setTexture(loadingPattenTextureName);
		add(loadingProgressBar);
	}

	void SceneSplash::onInitialAttach()
	{
		splashTransition = modules->animation->createTransitionForTimeScope(AW::TimeScope::Menu);
		splashTransition->listener = weak_from_this();

		modules->input->mouse->registerMouseButton(AWMouseButton::Left, baseSceneWeakThisRef());
		modules->input->keyboard->registerKey(AWKey::SPACE, baseSceneWeakThisRef());
	}

	void SceneSplash::onAttach()
	{
		state = 0;
		splashText->setAlpha(0.0);
		splashImage->setAlpha(0.0);
		titleGame->setAlpha(0.0);
		loadingProgressBar->setColor(150, 150, 150);
		loadingProgressBar->setScreenHeight(loadingBarHeight);
		splashTransition->startTargetlessTransition(splashTransitionTimeInSeconds * 1000.0);
		modules->physic->setWorldGravity(0, 0.0, -2.0);
	}

	void SceneSplash::onTransitionAnimationFrame(double position)
	{
		const auto currentTransitionSeconds = position * splashTransitionTimeInSeconds;
		const auto scaledMainPositionIn = AW::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorIn), 0.0, 1.0);
		const auto scaledMainPositionOut = AW::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactorOut), 0.0, 1.0);

		loadingProgressBar->setScreenWidth(getDisplayScreenWidth() * position);
		loadingProgressBar->topLeftAlignSelf(0.0, getDisplayScreenHeight() - loadingProgressBar->getScreenHeight());

		if (currentTransitionSeconds >= splashTransitionTimeInSeconds - 1.0)
		{
			const auto p = currentTransitionSeconds - (splashTransitionTimeInSeconds - 1.0);
			loadingProgressBar->setAlpha(AW::NumberHelper::clamp<double>(1.0 - p, 0.0, 1.0));
		}

		if (state == 0)
		{
			splashImage->setScreenPosition(getDisplayScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getDisplayScreenHeight() / 2.0 + splashText->getScreenHeight() + verticalOffset);
			splashImage->setAlpha(scaledMainPositionIn);

			splashText->toTopOf(splashImage);
			splashText->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 1)
		{
			splashImage->setScreenPosition(getDisplayScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getDisplayScreenHeight() / 2.0 + splashText->getScreenHeight() + verticalOffset);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashImage->setTexture(box2dLogoTextureName);
			}
		}
		else if (state == 2)
		{
			splashImage->setScreenPosition(getDisplayScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getDisplayScreenHeight() / 2.0 + splashText->getScreenHeight() + verticalOffset);
			splashImage->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 3)
		{
			splashImage->setScreenPosition(getDisplayScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getDisplayScreenHeight() / 2.0 + splashText->getScreenHeight() + verticalOffset);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);

			splashText->toTopOf(splashImage);
			splashText->setAlpha(1.0 - scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				splashText->setFontSize(titleFontSizeBig);
				const auto orgName = modules->gameConfig->getConfigString(Config::Param::organizationName);
				splashText->setText(orgName + " Presents");
				splashImage->setTexture(awGamesLogoTextureName);
				if (splashText->getScreenWidth() > getDisplayScreenWidth())
				{
					splashText->setFontSize(titleFontSizeSmall);
				}
			}
		}
		else if (state == 4)
		{
			splashText->setScreenPosition(getDisplayScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getDisplayScreenHeight() / 2.0 + splashImage->getScreenHalfHeight());
			splashText->setAlpha(scaledMainPositionIn);

			splashImage->toTopOf(splashText);
			splashImage->setAlpha(scaledMainPositionIn);

			tryToGotoNextState(position, 2.0);
		}
		else if (state == 5)
		{
			splashText->setAlpha(1.0 - scaledMainPositionOut);
			splashText->setScreenPosition(getDisplayScreenWidth() / 2.0 + (scaledMainPositionOut * fadeInHorMovement), getDisplayScreenHeight() / 2.0 + splashImage->getScreenHalfHeight());

			splashImage->toTopOf(splashText);
			splashImage->setAlpha(1.0 - scaledMainPositionOut);

			if (tryToGotoNextState(position, 1.0))
			{
				for (auto i = 0; i < numBlocksToMake; ++i)
				{
					const auto box = std::make_shared<AW::Box>();
					box->layoutSpace = AW::LayoutSpace::Screen;
					box->setColor(blockColorGenerator.getBlockColor());
					box->setScreenPosition(AW::NumberHelper::random(-100, modules->screen->getWidth() + 100), AW::NumberHelper::random(-100, -1000));
					add(box);
				}

				bottomCollider = std::make_shared<AW::Box>();
				bottomCollider->layoutSpace = AW::LayoutSpace::Screen;
				bottomCollider->visible = false;
				bottomCollider->setDynamic(false);
				bottomCollider->setScreenPositionSize(0.0, modules->screen->getHeight(), 8000.0, 20.0);
				add(bottomCollider);

				titleGame->visible = true;
			}
		}
		else if (state == 6)
		{
			titleGame->setScreenPosition(getDisplayScreenWidth() / 2.0 - ((1.0 - scaledMainPositionIn) * fadeInHorMovement), getDisplayScreenHeight() / 2.0);
			titleGame->setAlpha(scaledMainPositionIn);

			titleGameCollider->matchPosition(titleGame, 0.0, 50.0);
			titleGameCollider->setScreenX(getDisplayScreenWidth() / 2.0 - ((1.0 - 1.0) * fadeInHorMovement));

			titleGameCollider->setSize(titleGame);

			if (!titleGameCollider->isAttached())
			{
				add(titleGameCollider);
			}

			tryToGotoNextState(position, 6.0);
		}
		else if (state == 7)
		{
			titleGame->setScreenPosition(getDisplayScreenWidth() / 2.0 + (scaledMainPositionOut * 0.0), getDisplayScreenHeight() / 2.0);

			setAlpha(1.0 - scaledMainPositionOut);

			if (titleGameCollider->isAttached())
			{
				titleGameCollider->removeFromParent();
				bottomCollider->removeFromParent();
				modules->physic->setWorldGravity(0);
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
		destroyChildren();
		transitionToScene(SceneGame::MainMenu);
	}

	void SceneSplash::onMouseButtonLeftDown()
	{
		splashTransition->finish();
	}

	void SceneSplash::onKeyPressed(AWKey key)
	{
		splashTransition->finish();
	}

}
