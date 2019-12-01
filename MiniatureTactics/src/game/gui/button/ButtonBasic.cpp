#include "ButtonBasic.h"

#include "gui/Guis.h"

namespace MTGame
{
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

	void ButtonBasic::onCreateChildren()
	{
		label = std::make_shared<MT::Text>();
		label->name = "button_text";
		label->setFont("medium", 30);
		label->setText(text);
		label->setPosition(getWidth() / 2.0, getHeight() / 2.0);
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
