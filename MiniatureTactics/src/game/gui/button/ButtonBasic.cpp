#include "ButtonBasic.h"
#include "gui/Guis.h"

namespace
{
	const auto buttonBasicId = "button-basic";
	const auto mediumFontId = "medium";
	const auto enabledParamName = "button-basic-is-enabled";
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
				350,
				150
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
		setTexture("button-basic");
		setAnimationSet("button-basic");
		setDefaultAnimationName("default");

		setSize(350.0, 150.0);
		setScale(0.5);

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
	}

	void ButtonBasic::onCreateChildren()
	{
		label = std::make_shared<MT::Text>();
		label->name = "button_text";
		label->setFont("medium", 30);
		label->setText(text);
		label->setPosition(getWidth() / 2.0, getHeight() / 2.0);
		label->setColor(getColor());
		add(label);
	}

	void ButtonBasic::onChildrenHydrated()
	{
		label = findChildWithName<MT::Text>("button_text");
	}

	void ButtonBasic::onInitialAttach()
	{
		MT::Animated::onInitialAttach();

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
		isHovering =
			x < worldRect.x + worldRect.w && x > worldRect.x &&
			y < worldRect.y + worldRect.h && y > worldRect.y;
	}

	void ButtonBasic::onMouseButton(MT::MouseButton button, bool pressed)
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

				const auto listenerPtr = std::dynamic_pointer_cast<IButtonListener>(clickListener.lock());
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
	}
}
