#include "ScrollBarBasic.h"
#include "gui/Guis.h"

namespace
{
	const auto scrollbarBasicId = "scrollbar-basic";
	const auto enabledParamName = "scrollbar-basic-is-enabled";
}

namespace MTGame
{
	void ScrollBarBasic::loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle)
	{
		bundle->texture->loadTexture("res/game/img/ui/button/proto_button.png", scrollbarBasicId);

		auto animationSet = std::make_shared<MT::AnimationSet>();
		{
			int fps = 15;
			MT::RectI frameSize = {
				0,
				0,
				64,
				64
			};

			{
				auto anim = animationSet->startNewAnimation("default");
				anim->setFps(fps);
				anim->addGeneralFrames(0, 0, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("pressed");
				anim->setFps(fps);
				anim->addGeneralFrames(frameSize.w * 1, 0, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("hover");
				anim->setFps(fps);
				anim->addGeneralFrames(0, frameSize.h, frameSize.w, frameSize.h, 1);
			}
		}
		bundle->animation->addAnimationSet(animationSet, scrollbarBasicId);
	}

	ScrollBarBasic::ScrollBarBasic() : BaseGui(GuiScrollBar::ScrollBarBasic)
	{
		/*
		setTexture(scrollbarBasicId);
		setAnimationSet(scrollbarBasicId);
		setDefaultAnimationName("default");

		setCornerSize(16);
		*/

		enableSerialization<ScrollBarBasic>();
	}

	void ScrollBarBasic::setEnabled(bool flag)
	{
		serializationClient->setBool(enabledParamName, flag);
	}

	void ScrollBarBasic::onCreateChildren()
	{

	}

	void ScrollBarBasic::onChildrenHydrated()
	{
	}

	void ScrollBarBasic::onInitialAttach()
	{
		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, weak_from_this());
		enableEnterFrame();
	}

	void ScrollBarBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void ScrollBarBasic::checkIsHovering(int x, int y)
	{
		isHovering =
			x < worldRect.x + worldRect.w && x > worldRect.x &&
			y < worldRect.y + worldRect.h && y > worldRect.y;
	}

	void ScrollBarBasic::onMouseButton(MT::MouseButton button, bool pressed)
	{
		const auto isEnabled = serializationClient->getBool(enabledParamName, true);
		if (!isEnabled)
		{
			return;
		}

		switch (button)
		{
		case MT::MouseButton::Left:
			if (!pressed)
			{

				const auto listenerPtr = std::dynamic_pointer_cast<IGuiListener>(clickListener.lock());
				if (listenerPtr != nullptr && isPressed && isHovering)
				{
					listenerPtr->onButtonClicked(getId());
				}
			}

			isPressed = isHovering && pressed;
		}
	}

	void ScrollBarBasic::onEnterFrame(double frameTime)
	{
		checkIsHovering(modules->input->mouse->X(), modules->input->mouse->Y());

		/*
		if (isHovering)
		{
			if (isPressed)
			{
				play("pressed");
			}
			else
			{
				play("hover");
			}
		}
		else
		{
			play("default");
		}
		*/
	}
}
