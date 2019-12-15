#pragma once

#include <string>
#include <map>

namespace MTGame
{

	enum class GuiButton
	{
		ButtonBasic,
		CheckBoxBasic,
		RadioBoxBasic
	};

	enum class GuiCamera
	{
		CameraBasic
	};

	enum class GuiMenu
	{
		MenuGame
	};

	enum class GuiTransition
	{
		TransitionFade
	};

	enum class GuiHud
	{
		HudCombat,
		HudInventory
	};

	enum class GuiScrollBar
	{
		ScrollBarBasic
	};

	enum class GuiScrollArea
	{
		ScrollArea
	};

	class Guis
	{
	public:

		const std::map<GuiButton, std::string> buttons{
			{ GuiButton::ButtonBasic, "button-basic" },
			{ GuiButton::CheckBoxBasic, "check-box-basic" },
			{ GuiButton::RadioBoxBasic, "radio-box-basic" },
		};

		const std::map<GuiCamera, std::string> cameras{
			{ GuiCamera::CameraBasic, "camera-basic" },
		};

		const std::map<GuiMenu, std::string> menus{
			{ GuiMenu::MenuGame, "menu-game" },
		};

		const std::map<GuiTransition, std::string> transitions{
			{ GuiTransition::TransitionFade, "transition-fade" },
		};

		const std::map<GuiHud, std::string> huds{
			{ GuiHud::HudCombat, "hud-combat" },
			{ GuiHud::HudInventory, "hud-inventory" },
		};

		const std::map<GuiScrollBar, std::string> scrollbars{
			{ GuiScrollBar::ScrollBarBasic, "scrollbar-basic" }
		};

		const std::map<GuiScrollArea, std::string> scrollareas {
			{ GuiScrollArea::ScrollArea, "scroll-area-basic" }
		};

	};
}
