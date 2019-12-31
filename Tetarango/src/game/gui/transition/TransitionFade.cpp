#include "TransitionFade.h"

namespace
{
	const auto fadeRectName = "fade-rect";
	const auto fadeDurationParam = "t-fade-duration";
	const auto fadeTextureName = "fade-loading-texture";
	const auto loadingTransitionShaderName = "f-fade-loading";
	const auto loadingTransitionTranslateShaderName = "f-fade-loading-translate";
}

namespace AWGame
{

	TransitionFade::TransitionFade() : BaseGui(GuiTransition::TransitionFade)
	{
		registerGameObject<TransitionFade>();
	}

	void TransitionFade::onLoadResources()
	{
		modules->texture->loadTexture("res/image/loading/loading.png", fadeTextureName);
		modules->shader->loadShader("res/shader/fragment/loading-transition.glsl", loadingTransitionShaderName);
		modules->shader->loadShader("res/shader/fragment/loading-transition-translate.glsl", loadingTransitionTranslateShaderName);
	}

	void TransitionFade::onInitialAttach()
	{
		fadeTransition = modules->animation->createTransition();
		fadeTransition->listener = weak_from_this();
	}

	void TransitionFade::onCreateChildren()
	{
		fadeRectangle = std::make_shared<AW::Element>();
		fadeRectangle->renderTextureMode = AW::RenderTextureMode::LinearWrapping;
		fadeRectangle->name = fadeRectName;
		fadeRectangle->setTexture(fadeTextureName);
		setTransitionShader();
		add(fadeRectangle);
	}

	void TransitionFade::onChildrenHydrated()
	{
		fadeRectangle = findChildWithName<AW::Element>(fadeRectName);
		setTransitionShader();
	}

	void TransitionFade::setTransitionShader()
	{
		if (fadeRectangle != nullptr)
		{
			fadeRectangle->setFragmentShader(modules->shader->getShader({ "f-repeat", loadingTransitionTranslateShaderName, "element", loadingTransitionShaderName }, true));
			fadeRectangle->getFragmentShader()->setFloatIUParam("fRepeat", 64.0);
		}
	}

	void TransitionFade::onLayoutChildren()
	{
		fadeRectangle->setSizeAndCenter(this);
	}

	void TransitionFade::onTransitionCompleted(int transitionId)
	{
		const auto listenerPtr = listener.lock();

		if (!shouldFadeIn)
		{
			if (listenerPtr != nullptr)
			{
				listenerPtr->onFadeOut();
			}
		}
		else
		{
			if (listenerPtr != nullptr)
			{
				listenerPtr->onFadeIn();
			}
		}
	}

	void TransitionFade::setDuration(double duration)
	{
		serializationClient->setDouble(fadeDurationParam, duration);
	}

	double TransitionFade::getDuration()
	{
		return serializationClient->getDouble(fadeDurationParam, 1000.0);
	}

	void TransitionFade::fadeIn()
	{
		shouldFadeIn = true;
		fadeTransition->startTargetlessTransition(getDuration());
	}

	void TransitionFade::fadeInImmediately()
	{
		shouldFadeIn = true;
		fadeRectangle->getFragmentShader()->setFloatIUParam("fLoadingP", 1.0);
		onTransitionCompleted(-1);
	}

	void TransitionFade::fadeOut()
	{
		shouldFadeIn = false;
		fadeTransition->startTargetlessTransition(getDuration());
	}

	void TransitionFade::fadeOutImmediately()
	{
		shouldFadeIn = false;
		fadeRectangle->getFragmentShader()->setFloatIUParam("fLoadingP", 0.0);
		onTransitionCompleted(-1);
	}

	void TransitionFade::onTransitionAnimationFrame(double p)
	{
		const auto fP = shouldFadeIn ? p : (1 - p);
		fadeRectangle->getFragmentShader()->setFloatIUParam("fLoadingP", fP);
	}
}
