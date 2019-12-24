#include "TransitionFade.h"

namespace
{
	const auto fadeRectName = "fade-rect";
	const auto fadeDurationParam = "t-fade-duration";
}

namespace AWGame
{

	TransitionFade::TransitionFade() : BaseGui(GuiTransition::TransitionFade)
	{
		registerGameObject<TransitionFade>();
	}

	void TransitionFade::onInitialAttach()
	{
		fadeTransition = modules->animation->createTransition();
		fadeTransition->listener = weak_from_this();
	}

	void TransitionFade::onCreateChildren()
	{
		fadeRectangle = std::make_shared<AW::Rectangle>();
		fadeRectangle->name = fadeRectName;
		fadeRectangle->setColor(AW::Color::black());
		add(fadeRectangle);
	}

	void TransitionFade::onChildrenHydrated()
	{
		fadeRectangle = findChildWithName<AW::Rectangle>(fadeRectName);
	}

	void TransitionFade::onLayoutChildren()
	{
		fadeRectangle->matchSizeAndCenter(this);
	}

	void TransitionFade::onTransitionCompleted(int transitionId)
	{
		const auto listenerPtr = listener.lock();

		if (this->getAlpha() == 0.0)
		{
			visible = false;
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
		visible = true;
		fadeTransition->startTransition(std::dynamic_pointer_cast<AW::Renderable>(shared_from_this()), getDuration(), getRect(), 1.0);
	}

	void TransitionFade::fadeInImmediately()
	{
		visible = true;
		setAlpha(1.0);
	}

	void TransitionFade::fadeOut()
	{
		fadeTransition->startTransition(std::dynamic_pointer_cast<AW::Renderable>(shared_from_this()), getDuration(), getRect(), 0.0);
	}

	void TransitionFade::fadeOutImmediately()
	{
		visible = false;
		setAlpha(0.0);
	}

}
