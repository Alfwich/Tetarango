#include "ScrollBarBasic.h"
#include "gui/Guis.h"

namespace
{
	const auto scrollbarBasicTextureId = "scrollbar-basic";
	const auto scrollbarBasicBackgroundId = "scrollbar-basic";
	const auto scrollbarBasicScrollerId = "scrollbar-scroller-basic";

	const auto enabledParamName = "scrollbar-basic-is-enabled";
	const auto scrollbarHeightParamName = "scrollbar-height";
	const auto scrollbarPositionParamName = "scrollbar-pos";
	const auto orientationParamName = "scrollbar-orientation";
}

namespace MTGame
{
	ScrollBarBasic::ScrollBarBasic() : BaseGui(GuiScrollBar::ScrollBarBasic)
	{
		enableSerialization<ScrollBarBasic>();
	}

	void ScrollBarBasic::onLoadResources()
	{
		modules->texture->loadTexture("res/game/img/ui/scrollbar/scrollbar_basic.png", scrollbarBasicTextureId);

		{
			auto animationSet = std::make_shared<MT::AnimationSet>();
			{
				int fps = 15;
				MT::RectI frameSize = {
					0,
					0,
					40,
					40
				};

				{
					auto anim = animationSet->startNewAnimation("black");
					anim->setFps(fps);
					anim->addGeneralFrames(0, 0, frameSize.w, frameSize.h, 1);
				}

				{
					auto anim = animationSet->startNewAnimation("white");
					anim->setFps(fps);
					anim->addGeneralFrames(frameSize.w * 1, 0, frameSize.w, frameSize.h, 1);
				}
			}
			modules->animation->addAnimationSet(animationSet, scrollbarBasicBackgroundId);
		}

		{
			auto animationSet = std::make_shared<MT::AnimationSet>();
			{
				int fps = 15;
				MT::RectI frameSize = {
					3,
					40,
					34,
					24
				};

				{
					auto anim = animationSet->startNewAnimation("black");
					anim->setFps(fps);
					anim->addGeneralFrames(frameSize.x, frameSize.y, frameSize.w, frameSize.h, 1);
				}

				frameSize.x += 6 + frameSize.w;

				{
					auto anim = animationSet->startNewAnimation("white");
					anim->setFps(fps);
					anim->addGeneralFrames(frameSize.x, frameSize.y, frameSize.w, frameSize.h, 1);
				}
			}
			modules->animation->addAnimationSet(animationSet, scrollbarBasicScrollerId);
		}

	}

	void ScrollBarBasic::updateScrollerPosition(bool instant)
	{
		const auto scrollerSize = getScrollerHeight() * getHeight();
		auto targetRect = scroller->getRect();
		if (getHorizontal())
		{
			targetRect.y = getHalfHeight();
			targetRect.x = getScrollPosition() * (getWidth() - scroller->getWidth()) + scroller->getHalfWidth();
		}
		else
		{
			targetRect.x = getHalfWidth();
			targetRect.y = getScrollPosition() * (getHeight() - scroller->getHeight()) + scroller->getHalfHeight();
		}
		scrollerTransition->startTransition(scroller, instant ? 0.0 : 50.0, targetRect);
	}

	void ScrollBarBasic::setEnabled(bool flag)
	{
		serializationClient->setBool(enabledParamName, flag);
	}

	void ScrollBarBasic::setScrollerHeight(double height)
	{
		serializationClient->setDouble(scrollbarHeightParamName, MT::NumberHelper::clamp<double>(height, 0.0, 1.0));
		layout();
	}

	double ScrollBarBasic::getScrollerHeight()
	{
		return serializationClient->getDouble(scrollbarHeightParamName, 0.1);
	}

	void ScrollBarBasic::setHorizontal(bool flag)
	{
		serializationClient->setBool(orientationParamName, flag);
	}

	bool ScrollBarBasic::getHorizontal()
	{
		return serializationClient->getBool(orientationParamName, false);
	}

	void ScrollBarBasic::setScrollPositionInstantly(double pos)
	{
		const auto value = MT::NumberHelper::clamp<double>(pos, 0.0, 1.0);
		serializationClient->setDouble(scrollbarPositionParamName, value);

		updateScrollerPosition(true);

		const auto notifyPtr = std::dynamic_pointer_cast<IGuiListener>(scrollListener.lock());
		if (isPressed && notifyPtr != nullptr)
		{
			notifyPtr->onScrollBarScroll(id, value);
		}
	}

	void ScrollBarBasic::setScrollPosition(double pos)
	{
		const auto value = MT::NumberHelper::clamp<double>(pos, 0.0, 1.0);
		serializationClient->setDouble(scrollbarPositionParamName, value);

		updateScrollerPosition();

		const auto notifyPtr = std::dynamic_pointer_cast<IGuiListener>(scrollListener.lock());
		if (isPressed && notifyPtr != nullptr)
		{
			notifyPtr->onScrollBarScroll(id, value);
		}
	}

	double ScrollBarBasic::getScrollPosition()
	{
		return serializationClient->getDouble(scrollbarPositionParamName, 0.0);
	}

	void ScrollBarBasic::onCreateChildren()
	{
		background = std::make_shared<MT::NineSlice>();
		background->name = scrollbarBasicBackgroundId;
		background->setTexture(scrollbarBasicTextureId);
		background->setAnimationSet(scrollbarBasicBackgroundId);
		background->setCornerSize(8);
		add(background);
		background->play("black");

		scroller = std::make_shared<MT::NineSlice>();
		scroller->name = scrollbarBasicScrollerId;
		scroller->setTexture(scrollbarBasicTextureId);
		scroller->setAnimationSet(scrollbarBasicScrollerId);
		scroller->setCornerSize(5);
		scroller->zIndex = 1;
		add(scroller);
		scroller->play("black");
	}

	void ScrollBarBasic::onChildrenHydrated()
	{
		background = findChildWithName<MT::NineSlice>(scrollbarBasicBackgroundId);
		scroller = findChildWithName<MT::NineSlice>(scrollbarBasicScrollerId);
	}

	void ScrollBarBasic::onInitialAttach()
	{
		scrollerTransition = modules->animation->createTransition();
		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, weak_from_this());
		enableEnterFrame();
	}

	void ScrollBarBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void ScrollBarBasic::onLayoutChildren()
	{
		background->matchSize(this);
		background->centerAlignSelf();
		if (getHorizontal())
		{
			const auto scrollerHeight = getScrollerHeight() * getWidth();
			scroller->setHeight(getHeight());
			scroller->setWidth(scrollerHeight);
		}
		else
		{
			const auto scrollerHeight = getScrollerHeight() * getHeight();
			scroller->setHeight(scrollerHeight);
			scroller->setWidth(getWidth());
		}
		scroller->centerAlignSelf();
		updateScrollerPosition();
	}

	void ScrollBarBasic::checkIsHovering(int x, int y)
	{
		const auto rect = getFirstNonUnspecifiedRenderPositionMode() == MT::RenderPositionMode::Unspecified ? background->getScreenRect() : background->getWorldRect();

		isHovering =
			x < rect->x + rect->w && x > rect->x &&
			y < rect->y + rect->h && y > rect->y;
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

				wasPressed = false;
			}

			isPressed = isHovering && pressed;

			if (isPressed) 
			{
				startMousePositionOnClick = getHorizontal() ? modules->input->mouse->X() : modules->input->mouse->Y();
				startMousePositionScrollPosition = getScrollPosition();
			}
		}
	}

	void ScrollBarBasic::onEnterFrame(double frameTime)
	{
		const auto mouseY = modules->input->mouse->Y();
		const auto mouseX = modules->input->mouse->X();
		checkIsHovering(mouseX, mouseY);

		if (isHovering || (isPressed && wasPressed))
		{
			if (isPressed)
			{
				wasPressed = true;
				if (getHorizontal())
				{
					setScrollPosition(startMousePositionScrollPosition + ((mouseX - startMousePositionOnClick) / getWidth()));
				}
				else
				{
					setScrollPosition(startMousePositionScrollPosition + ((mouseY - startMousePositionOnClick) / getHeight()));
				}
			}
		}

		if (isHovering || (isPressed && wasPressed) || !getInputEnabled())
		{
			scroller->setAlpha(1.0);
		}
		else
		{
			scroller->setAlpha(0.80);
		}
	}
}
