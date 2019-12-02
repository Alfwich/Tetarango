#include "CheckBoxBasic.h"
#include "gui/Guis.h"

namespace
{
	const auto checkBoxBasicId = "check-box-basic";
	const auto isCheckedParamName = "check-box-basic-is-checked";
}

namespace MTGame
{
	void CheckBoxBasic::loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle)
	{
		bundle->texture->loadTexture("res/game/img/ui/button/checkbox_basic.png", checkBoxBasicId);

		auto animationSet = std::make_shared<MT::AnimationSet>();
		{
			MT::RectI frameSize = {
				0,
				0,
				40,
				40
			};

			{

				auto anim = animationSet->startNewAnimation("default");
				anim->addGeneralFrames(0, 0, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("hover");
				anim->addGeneralFrames(frameSize.w * 1, 0, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("pressed");
				anim->addGeneralFrames(frameSize.w * 2, 0, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("default-checked");
				anim->addGeneralFrames(0, frameSize.h, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("hover-checked");
				anim->addGeneralFrames(frameSize.w * 1, frameSize.h, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("pressed-checked");
				anim->addGeneralFrames(frameSize.w * 2, frameSize.h, frameSize.w, frameSize.h, 1);
			}
		}
		bundle->animation->addAnimationSet(animationSet, checkBoxBasicId);

	}

	CheckBoxBasic::CheckBoxBasic() : BaseGui(GuiButton::CheckBoxBasic)
	{
		setTexture(checkBoxBasicId);
		setAnimationSet(checkBoxBasicId);
		setDefaultAnimationName("default");

		setSize(33.0, 33.0);

		enableSerialization<CheckBoxBasic>();
	}

	void CheckBoxBasic::setText(std::string text)
	{
		this->text = text;

		if (label != nullptr)
		{
			label->setText(text);
		}
	}

	std::string CheckBoxBasic::getText()
	{
		return text;
	}

	bool CheckBoxBasic::getChecked()
	{
		return serializationClient->getBool(isCheckedParamName, false);
	}

	void CheckBoxBasic::setChecked(bool isChecked)
	{
		serializationClient->setBool(isCheckedParamName, isChecked);
	}

	void CheckBoxBasic::onCreateChildren()
	{
		label = std::make_shared<MT::Text>();
		label->name = "button_text";
		label->setFont("medium", 30);
		label->setText(text);
		label->setPosition(getWidth() / 2.0, -label->getHalfHeight());
		label->setColor(getColor());
		add(label);
	}

	void CheckBoxBasic::onChildrenHydrated()
	{
		label = findChildWithName<MT::Text>("button_text");
	}

	void CheckBoxBasic::onInitialAttach()
	{
		MT::Animated::onInitialAttach();

		enableEnterFrame();

		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, weak_from_this());
	}

	void CheckBoxBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void CheckBoxBasic::checkIsHovering(int x, int y)
	{
		isHovering =
			x < worldRect.x + worldRect.w && x > worldRect.x &&
			y < worldRect.y + worldRect.h && y > worldRect.y;
	}

	void CheckBoxBasic::onMouseButton(MT::MouseButton button, bool pressed)
	{
		switch (button)
		{
		case MT::MouseButton::Left:
			if (!pressed)
			{
				if (isHovering)
				{
					setChecked(!getChecked());

					const auto listenerPtr = std::dynamic_pointer_cast<IButtonListener>(clickListener.lock());
					if (listenerPtr != nullptr && isPressed && isHovering)
					{
						listenerPtr->onButtonClicked(getId());
					}
				}
			}

			isPressed = isHovering && pressed;
		}
	}

	void CheckBoxBasic::onEnterFrame(double frameTime)
	{
		checkIsHovering(modules->input->mouse->X(), modules->input->mouse->Y());

		if (!getChecked())
		{
			if (isHovering && isPressed)
			{
				play("pressed");
			}
			else if (isHovering)
			{
				play("hover");
			}
			else
			{
				play("default");
			}
		}
		else
		{
			if (isHovering && isPressed)
			{
				play("pressed-checked");
			}
			else if (isHovering)
			{
				play("hover-checked");
			}
			else
			{
				play("default-checked");
			}
		}
	}
}