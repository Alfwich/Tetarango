#include "CheckBoxBasic.h"
#include "gui/Guis.h"

namespace
{
	const auto checkBoxBasicId = "check-box-basic";
	const auto isCheckedParamName = "check-box-basic-is-checked";
	const auto isLockedParamName = "check-box-is-locked";
	const std::vector<std::string> states = {
		"default",
		"hover",
		"pressed",
		"default-checked",
		"hover-checked",
		"pressed-checked",
	};
	const std::vector<std::string> skins = {
		"checkbox",
		"radio"
	};
	const auto checkboxTexturePath = "res/game/img/ui/button/checkbox_basic.png";
}

namespace MTGame
{
	void CheckBoxBasic::loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle)
	{
		bundle->texture->loadTexture(checkboxTexturePath, checkBoxBasicId);

		auto animationSet = std::make_shared<MT::AnimationSet>();
		{
			auto row = 0;
			auto col = 0;

			MT::RectI frameSize = {
				0,
				0,
				40,
				40
			};

			for (const auto skin : skins)
			{
				for (const auto state : states)
				{
					auto anim = animationSet->startNewAnimation(skin + state);
					anim->addGeneralFrames((col++) * frameSize.w, row * frameSize.h, frameSize.w, frameSize.h, 1);

					if (col == 3)
					{
						row++;
						col = 0;
					}
				}
			}
		}

		bundle->animation->addAnimationSet(animationSet, checkBoxBasicId);
	}

	CheckBoxBasic::CheckBoxBasic(GuiButton configuration) : BaseGui(configuration)
	{
		switch (configuration)
		{
		case GuiButton::RadioBoxBasic:
			setAnimationPrefix(skins[1]);
			break;

		default:
		case GuiButton::CheckBoxBasic:
			setAnimationPrefix(skins[0]);
			break;
		}

		setTexture(checkBoxBasicId);
		setAnimationSet(checkBoxBasicId);
		setDefaultAnimationName("default");

		setSize(40.0, 40.0);

		enableSerialization<CheckBoxBasic>();
	}

	CheckBoxBasic::CheckBoxBasic() : CheckBoxBasic(GuiButton::CheckBoxBasic) {}

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

	void CheckBoxBasic::setEnabled(bool flag)
	{
		serializationClient->setBool(isLockedParamName, flag);
	}

	void CheckBoxBasic::onCreateChildren()
	{
		label = std::make_shared<MT::Text>();
		label->name = "button_text";
		label->setFont("medium", 30);
		label->setText(text);
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

	void CheckBoxBasic::onLayoutChildren()
	{
		label->centerAlignSelf(getWidth() + 20.0, 0.0);
	}

	void CheckBoxBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void CheckBoxBasic::checkIsHovering(int x, int y)
	{
		const auto rect = getFirstNonUnspecifiedRenderPositionMode() == MT::RenderPositionMode::Unspecified ? screenRect : worldRect;

		isHovering =
			x < rect.x + rect.w && x > rect.x &&
			y < rect.y + rect.h && y > rect.y;
	}

	void CheckBoxBasic::onMouseButton(MT::MouseButton button, bool pressed)
	{
		const auto isLocked = serializationClient->getBool(isLockedParamName, false);
		if (isLocked)
		{
			return;
		}

		switch (button)
		{
		case MT::MouseButton::Left:
			if (!pressed)
			{
				if (isHovering)
				{
					setChecked(!getChecked());

					const auto listenerPtr = std::dynamic_pointer_cast<IGuiListener>(clickListener.lock());
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