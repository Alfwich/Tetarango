#include "TransitionFade.h"

namespace MTGame
{

	TransitionFade::TransitionFade() : BaseGui(GuiTransition::TransitionFade)
	{
		enableSerialization<TransitionFade>();
	}

	void TransitionFade::onInitialAttach()
	{
		fadeTransition = modules->animation->createTransition();
		fadeTransition->listener = std::dynamic_pointer_cast<MT::INotifyOnTransition>(shared_from_this());
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
		serializationClient->setDouble("fade_duration", duration);
	}

	void TransitionFade::fadeIn()
	{
		visible = true;
		fadeTransition->startTransition(std::dynamic_pointer_cast<MT::Renderable>(shared_from_this()), serializationClient->getDouble("fade_duration"), getRect(), 1.0);
	}

	void TransitionFade::fadeInImmediately()
	{
		visible = true;
		setAlpha(1.0);
	}

	void TransitionFade::fadeOut()
	{
		fadeTransition->startTransition(std::dynamic_pointer_cast<MT::Renderable>(shared_from_this()), serializationClient->getDouble("fade_duration"), getRect(), 0.0);
	}

	void TransitionFade::fadeOutImmediately()
	{
		visible = false;
		setAlpha(0.0);
	}

}
