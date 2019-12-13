#include "SceneSplash.h"
#include <iostream>

namespace
{
	const auto sdlLogoTextureName = "splash-sdl-logo";
	const auto openGLLogoTextureName = "splash-opengl-logo";
	const auto splashTransitionTimeInSeconds = 12.5;
	const auto fadeInHorMovement = 300.0;
	const auto animationScalingFactor = 3.0;
	const auto animationTiming = 1.0;
	const auto titleFontSizeBig = 150;
	const auto titleFontSizeSmall = 75;
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
	}

	void SceneSplash::onCreateChildren()
	{
		splashText = std::make_shared<MT::Text>();
		splashText->setFont("medium", titleFontSizeBig);
		splashText->setText("built with");
		add(splashText);

		splashImage = std::make_shared<MT::Element>();
		splashImage->setTexture(sdlLogoTextureName);
		splashImage->setMatchSizeToTexture(true);
		add(splashImage);
	}

	void SceneSplash::onInitialAttach()
	{
		splashTransition = modules->animation->createTransitionForTimeScope(MT::TimeScope::Menu);
		splashTransition->listener = std::dynamic_pointer_cast<MT::INotifyOnTransition>(shared_from_this());

		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, baseSceneWeakThisRef());
		modules->input->keyboard->registerKey(SDL_SCANCODE_SPACE, baseSceneWeakThisRef());
	}

	void SceneSplash::onAttach()
	{
		state = 0;
		splashText->setAlpha(0.0);
		splashTransition->startTargetlessTransition(splashTransitionTimeInSeconds * 1000.0);
	}

	void SceneSplash::onTransitionAnimationFrame(double position)
	{
		const auto currentTransitionSeconds = position * splashTransitionTimeInSeconds;
		const auto currentStateTime = currentTransitionSeconds - animationOffset * splashTransitionTimeInSeconds;
		std::cout << currentStateTime << std::endl;
		if (state == 0)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashImage->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledPosition) * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight());
			splashText->toTopOf(splashImage);

			splashText->setAlpha(scaledPosition);
			splashImage->setAlpha(scaledPosition);

			if (currentStateTime > 2.0)
			{
				animationOffset = position;
				state = 1;
			}
		}
		else if (state == 1)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashImage->setPosition(getScreenWidth() / 2.0 + (scaledPosition * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight());
			splashImage->setAlpha(1.0 - scaledPosition);

			if (currentStateTime > 1.0)
			{
				splashImage->setTexture(openGLLogoTextureName);
				animationOffset = position;
				state = 2;
			}
		}
		else if (state == 2)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashImage->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledPosition) * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight());
			splashImage->setAlpha(scaledPosition);

			if (currentStateTime > 2.0)
			{
				animationOffset = position;
				state = 3;
			}
		}
		else if (state == 3)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashImage->setPosition(getScreenWidth() / 2.0 + (scaledPosition * fadeInHorMovement), getScreenHeight() / 2.0 + splashText->getHeight());
			splashImage->setAlpha(1.0 - scaledPosition);
			splashText->toTopOf(splashImage);
			splashText->setAlpha(1.0 - scaledPosition);

			if (currentStateTime > 1.0)
			{
				splashText->setFontSize(titleFontSizeBig);
				splashText->setText("AW Games Presents");
				if (splashText->getWidth() > getScreenWidth())
				{
					splashText->setFontSize(titleFontSizeSmall);
				}
				animationOffset = position;
				state = 4;
			}
		}
		else if (state == 4)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashText->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledPosition) * fadeInHorMovement), getScreenHeight() / 2.0);
			splashText->setAlpha(scaledPosition);

			if (currentStateTime > 2.0)
			{
				animationOffset = position;
				state = 5;
			}
		}
		else if (state == 5)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashText->setPosition(getScreenWidth() / 2.0 + (scaledPosition * fadeInHorMovement), getScreenHeight() / 2.0);
			splashText->setAlpha(1.0 - scaledPosition);

			if (currentStateTime > 1.0)
			{
				splashText->setFontSize(titleFontSizeBig);
				splashText->setText(modules->gameConfig->getConfigString(Config::Param::gameName));
				if (splashText->getWidth() > getScreenWidth())
				{
					splashText->setFontSize(titleFontSizeSmall);
				}
				animationOffset = position;
				state = 6;
			}
		}
		else if (state == 6)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashText->setPosition(getScreenWidth() / 2.0 - ((1.0 - scaledPosition) * fadeInHorMovement), getScreenHeight() / 2.0);
			splashText->setAlpha(scaledPosition);

			if (currentStateTime > 2.0)
			{
				animationOffset = position;
				state = 7;
			}
		}
		else if (state == 7)
		{
			const auto scaledPosition = MT::NumberHelper::clamp(std::pow((position - animationOffset) * splashTransitionTimeInSeconds, animationScalingFactor), 0.0, 1.0);
			splashText->setPosition(getScreenWidth() / 2.0 + (scaledPosition * fadeInHorMovement), getScreenHeight() / 2.0);
			splashText->setAlpha(1.0 - scaledPosition);
		}
	}

	void SceneSplash::onTransitionCompleted()
	{
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
