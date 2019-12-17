#include "CheckBoxBasic.h"
#include "gui/Guis.h"
#include "ui/renderable/element/Rectangle.h"

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

namespace AWGame
{
	CheckBoxBasic::CheckBoxBasic(GuiButton configuration) : BaseGui(configuration)
	{
		renderTextureMode = AW::RenderTextureMode::BilinearNoWrap;
		registerSerialization<CheckBoxBasic>();
	}

	CheckBoxBasic::CheckBoxBasic() : CheckBoxBasic(GuiButton::CheckBoxBasic) {}

	void CheckBoxBasic::onLoadResources()
	{
		modules->texture->loadTexture(checkboxTexturePath, checkBoxBasicId);

		auto animationSet = std::make_shared<AW::AnimationSet>();
		{
			auto row = 0;
			auto col = 0;

			AW::RectI frameSize = {
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

		modules->animation->addAnimationSet(animationSet, checkBoxBasicId);
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

	void CheckBoxBasic::setEnabled(bool flag)
	{
		serializationClient->setBool(isLockedParamName, flag);
	}

	void CheckBoxBasic::onCreateChildren()
	{
		checkbox = std::make_shared<AW::Animated>();
		if (guiBaseName == getGuis().buttons.at(GuiButton::RadioBoxBasic))
		{
			checkbox->setAnimationPrefix(skins[1]);
		}
		else
		{
			checkbox->setAnimationPrefix(skins[0]);
		}
		checkbox->setTexture(checkBoxBasicId);
		checkbox->name = "checkbox";
		checkbox->setAnimationSet(checkBoxBasicId);
		checkbox->setDefaultAnimationName("default");
		checkbox->setSize(40.0, 40.0);
		checkbox->topLeftAlignSelf();
		add(checkbox);

		label = std::make_shared<AW::Text>();
		label->name = "button_text";
		label->setFont("medium", 24);
		label->setText(text);
		label->setColor(getColor());
		label->topLeftAlignSelf(45.0);
		add(label);
	}

	void CheckBoxBasic::onChildrenHydrated()
	{
		checkbox = findChildWithName<AW::Animated>("checkbox");
		label = findChildWithName<AW::Text>("button_text");
	}

	void CheckBoxBasic::onInitialAttach()
	{
		enableEnterFrame();
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());
	}

	void CheckBoxBasic::onLayoutChildren()
	{
		label->toRightOf(checkbox, 5.0);
		resizeSelfToChildrenAndCenterChildren();
	}

	void CheckBoxBasic::onDetach()
	{
		isPressed = false;
		isHovering = false;
	}

	void CheckBoxBasic::checkIsHovering(int x, int y)
	{
		const auto rect = getFirstNonUnspecifiedRenderPositionMode() == AW::RenderPositionMode::Unspecified ? screenRect : worldRect;

		isHovering =
			x < rect.x + rect.w && x > rect.x &&
			y < rect.y + rect.h && y > rect.y;
	}

	void CheckBoxBasic::onMouseButton(AW::MouseButton button, bool pressed)
	{
		const auto isLocked = serializationClient->getBool(isLockedParamName, false);
		if (isLocked)
		{
			return;
		}

		switch (button)
		{
		case AW::MouseButton::Left:
			if (!pressed)
			{
				if (isHovering && isPressed)
				{
					const auto listenerPtr = std::dynamic_pointer_cast<IGuiListener>(clickListener.lock());
					if (listenerPtr != nullptr)
					{
						listenerPtr->onButtonClicked(getId());
					}
					else
					{
						setChecked(!getChecked());
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
				checkbox->play("pressed");
			}
			else if (isHovering)
			{
				checkbox->play("hover");
			}
			else
			{
				checkbox->play("default");
			}
		}
		else
		{
			if (isHovering && isPressed)
			{
				checkbox->play("pressed-checked");
			}
			else if (isHovering)
			{
				checkbox->play("hover-checked");
			}
			else
			{
				checkbox->play("default-checked");
			}
		}
	}
}