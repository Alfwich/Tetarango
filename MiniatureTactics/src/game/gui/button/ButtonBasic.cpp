#include "ButtonBasic.h"
#include "gui/Guis.h"

namespace
{
	const auto buttonBasicId = "button-basic";
	const auto mediumFontId = "medium";
	const auto enabledParamName = "button-basic-is-enabled";
	const auto labelName = "button-basic-label";
	const auto backgroundName = "button-basic-background";
}

namespace MTGame
{
	void ButtonBasic::loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle)
	{
		bundle->font->loadFont("res/game/font/Roboto-Medium.ttf", mediumFontId);
		bundle->texture->loadTexture("res/game/img/ui/button/proto_button.png", buttonBasicId);

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
		bundle->animation->addAnimationSet(animationSet, buttonBasicId);
	}

	ButtonBasic::ButtonBasic() : BaseGui(GuiButton::ButtonBasic)
	{
		setSize(180, 80);
		enableSerialization<ButtonBasic>();
	}

	void ButtonBasic::setText(std::string text)
	{
		this->text = text;

		if (label != nullptr)
		{
			label->setText(text);
		}
	}

	std::string ButtonBasic::getText()
	{
		return text;
	}

	void ButtonBasic::setEnabled(bool flag)
	{
		serializationClient->setBool(enabledParamName, flag);

		if (flag)
		{
			setColor(255, 255, 255);
		}
		else
		{
			setColor(64, 64, 64);
		}
	}

	bool ButtonBasic::getEnabled()
	{
		return serializationClient->getBool(enabledParamName, true);
	}

	void ButtonBasic::onCreateChildren()
	{
		background = std::make_shared<MT::NineSlice>();
		background->setTexture(buttonBasicId);
		background->setAnimationSet(buttonBasicId);
		background->matchSizeAndCenter(this);
		background->setCornerSize(16);
		background->name = backgroundName;
		add(background);

		label = std::make_shared<MT::Text>();
		label->name = labelName;
		label->setFont("medium", 30);
		label->setText(text);
		label->setPosition(getWidth() / 2.0, getHeight() / 2.0);
		add(label);
	}

	void ButtonBasic::onChildrenHydrated()
	{
		label = findChildWithName<MT::Text>(labelName);
		background = findChildWithName<MT::NineSlice>(backgroundName);
	}

	void ButtonBasic::onInitialAttach()
	{
		enableEnterFrame();

		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, weak_from_this());
	}

	void ButtonBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void ButtonBasic::checkIsHovering(int x, int y)
	{
		const auto rect = getFirstNonUnspecifiedRenderPositionMode() == MT::RenderPositionMode::Unspecified ? screenRect : worldRect;

		isHovering =
			x < rect.x + rect.w && x > rect.x &&
			y < rect.y + rect.h && y > rect.y;
	}

	void ButtonBasic::onMouseButton(MT::MouseButton button, bool pressed)
	{
		const auto isEnabled = getEnabled();
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

	void ButtonBasic::onEnterFrame(double frameTime)
	{
		checkIsHovering(modules->input->mouse->X(), modules->input->mouse->Y());

		if (isHovering)
		{
			if (isPressed)
			{
				background->play("pressed");
			}
			else
			{
				background->play("hover");
			}
		}
		else
		{
			background->play("default");
		}
	}
}
