#include "ButtonBasic.h"
#include "gui/Guis.h"

namespace
{
	const auto buttonBasicId = "button-basic";
	const auto mediumFontId = "medium";
	const auto enabledParamName = "button-basic-is-enabled";
	const auto fontSizeParamName = "button-basic-font-size";
	const auto labelName = "button-basic-label";
	const auto backgroundName = "button-basic-background";
}

namespace AWGame
{
	ButtonBasic::ButtonBasic() : BaseGui(GuiButton::ButtonBasic)
	{
		setScreenSize(180, 80);
		GORegister(ButtonBasic);
	}

	void ButtonBasic::onLoadResources()
	{
		modules->texture->loadTexture("res/image/ui/button/proto_button.png", buttonBasicId);

		auto animationSet = std::make_shared<AW::AnimationSet>();
		{
			int fps = 15;
			AW::RectI frameSize = {
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
		modules->animation->addAnimationSet(animationSet, buttonBasicId);
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

	void ButtonBasic::setFontSize(int size)
	{
		serializationClient->setInt(fontSizeParamName, size);

		if (label != nullptr)
		{
			label->setFontSize(size);
		}
	}

	int ButtonBasic::getFontSize()
	{
		return serializationClient->getInt(fontSizeParamName, 30);
	}

	void ButtonBasic::setEnabled(bool flag)
	{
		serializationClient->setBool(enabledParamName, flag);
	}

	bool ButtonBasic::getEnabled()
	{
		return serializationClient->getBool(enabledParamName, true);
	}

	void ButtonBasic::onCreateChildren()
	{
		background = std::make_shared<AW::NineSlice>();
		background->setTexture(buttonBasicId);
		background->setAnimationSet(buttonBasicId);
		background->setCornerSize(16);
		background->name = backgroundName;
		add(background);

		label = std::make_shared<AW::Text>();
		label->name = labelName;
		label->setFont("medium", getFontSize());
		label->setTextColor(0, 0, 0);
		label->setText(text);
		add(label);
	}

	void ButtonBasic::onLayoutChildren()
	{
		background->setSizeAndCenter(this);
		label->setScreenPosition(getScreenWidth() / 2.0, getScreenHeight() / 2.0);
	}

	void ButtonBasic::onChildrenHydrated()
	{
		label = findChildWithName<AW::Text>(labelName);
		background = findChildWithName<AW::NineSlice>(backgroundName);
	}

	void ButtonBasic::onInitialAttach()
	{
		enableEnterFrame();

		modules->input->mouse->registerMouseButton(AWMouseButton::Left, weak_from_this());
	}

	void ButtonBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void ButtonBasic::checkIsHovering(int x, int y)
	{
		auto r = screenRect;

		isHovering =
			x < r.x + r.w && x > r.x &&
			y < r.y + r.h && y > r.y;
	}

	void ButtonBasic::onMouseButton(AWMouseButton button, bool pressed)
	{
		const auto isEnabled = getEnabled();
		if (!isEnabled)
		{
			return;
		}

		switch (button)
		{
		case AWMouseButton::Left:
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

	void ButtonBasic::onEnterFrame(const double& frameTime)
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
