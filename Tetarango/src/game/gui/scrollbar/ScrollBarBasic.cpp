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

namespace AWGame
{
	ScrollBarBasic::ScrollBarBasic() : BaseGui(GuiScrollBar::ScrollBarBasic)
	{
		renderTextureMode = AW::RenderTextureMode::BilinearNoWrap;
		registerGameObject<ScrollBarBasic>("scroll-bar-basic");
	}

	void ScrollBarBasic::onLoadResources()
	{
		modules->texture->loadTexture("res/image/ui/scrollbar/scrollbar_basic.png", scrollbarBasicTextureId);

		{
			auto animationSet = std::make_shared<AW::AnimationSet>();
			{
				int fps = 15;
				AW::RectI frameSize = {
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
			auto animationSet = std::make_shared<AW::AnimationSet>();
			{
				int fps = 15;
				AW::RectI frameSize = {
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
		serializationClient->setDouble(scrollbarHeightParamName, AW::NumberHelper::clamp<double>(height, 0.0, 1.0));
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
		const auto value = AW::NumberHelper::clamp<double>(pos, 0.0, 1.0);
		serializationClient->setDouble(scrollbarPositionParamName, value);

		updateScrollerPosition(true);

		const auto notifyPtr = std::dynamic_pointer_cast<IGuiListener>(scrollListener.lock());
		if (isPressed && notifyPtr != nullptr)
		{
			notifyPtr->onScrollBarScroll(getId(), value);
		}
	}

	void ScrollBarBasic::setScrollPosition(double pos)
	{
		const auto value = AW::NumberHelper::clamp<double>(pos, 0.0, 1.0);
		serializationClient->setDouble(scrollbarPositionParamName, value);

		updateScrollerPosition();

		const auto notifyPtr = std::dynamic_pointer_cast<IGuiListener>(scrollListener.lock());
		if (isPressed && notifyPtr != nullptr)
		{
			notifyPtr->onScrollBarScroll(getId(), value);
		}
	}

	double ScrollBarBasic::getScrollPosition()
	{
		return serializationClient->getDouble(scrollbarPositionParamName, 0.0);
	}

	void ScrollBarBasic::onCreateChildren()
	{
		background = std::make_shared<AW::NineSlice>();
		background->name = scrollbarBasicBackgroundId;
		background->setTexture(scrollbarBasicTextureId);
		background->setAnimationSet(scrollbarBasicBackgroundId);
		background->setCornerSize(8);
		add(background);
		background->play("black");

		scroller = std::make_shared<AW::NineSlice>();
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
		background = findChildWithName<AW::NineSlice>(scrollbarBasicBackgroundId);
		scroller = findChildWithName<AW::NineSlice>(scrollbarBasicScrollerId);
	}

	void ScrollBarBasic::onInitialAttach()
	{
		scrollerTransition = modules->animation->createTransition();
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());
		enableEnterFrame();
	}

	void ScrollBarBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void ScrollBarBasic::onLayoutChildren()
	{
		background->setSize(this);
		background->topLeftAlignSelf();
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

		updateScrollerPosition(true);
	}

	void ScrollBarBasic::checkIsHovering(int x, int y)
	{
		auto rect = screenRect;

		isHovering =
			x < rect.x + rect.w && x > rect.x &&
			y < rect.y + rect.h && y > rect.y;
	}

	void ScrollBarBasic::onMouseButton(AW::MouseButton button, bool pressed)
	{
		const auto isEnabled = serializationClient->getBool(enabledParamName, true);
		if (!isEnabled)
		{
			return;
		}

		switch (button)
		{
		case AW::MouseButton::Left:
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
				if (getHorizontal())
				{
					startMousePositionOnClick = modules->input->mouse->X() - (modules->input->mouse->X() - scroller->getScreenRect()->x) + scroller->getHalfWidth();
				}
				else
				{
					startMousePositionOnClick = modules->input->mouse->Y() - (modules->input->mouse->Y() - scroller->getScreenRect()->y) + scroller->getHalfHeight();
				}
				startMousePositionScrollPosition = getScrollPosition();
			}
		}
	}

	void ScrollBarBasic::onEnterFrame(const double& frameTime)
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
					setScrollPosition(startMousePositionScrollPosition + ((mouseX - startMousePositionOnClick) / (getWidth() - scroller->getWidth())));
				}
				else
				{
					setScrollPosition(startMousePositionScrollPosition + ((mouseY - startMousePositionOnClick) / (getHeight() - scroller->getHeight())));
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
