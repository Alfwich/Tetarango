#include "BaseGui.h"

#include <string>

#include "Guis.h"

namespace
{
	const AWGame::Guis config;
}

namespace AWGame
{
	const Guis& BaseGui::getGuis()
	{
		return config;
	}

	BaseGui::BaseGui(GuiButton button) :
		guiBaseName(config.buttons.at(button)),
		guiType(GuiType::Button) {}

	BaseGui::BaseGui(GuiCamera camera) :
		guiBaseName(config.cameras.at(camera)),
		guiType(GuiType::Camera) {}

	BaseGui::BaseGui(GuiMenu menu) :
		guiBaseName(config.menus.at(menu)),
		guiType(GuiType::Menu) {}

	BaseGui::BaseGui(GuiTransition transition) :
		guiBaseName(config.transitions.at(transition)),
		guiType(GuiType::Transition) {}

	BaseGui::BaseGui(GuiHud hud) :
		guiBaseName(config.huds.at(hud)),
		guiType(GuiType::Hud) {}

	BaseGui::BaseGui(GuiScrollBar scrollbar) :
		guiBaseName(config.scrollbars.at(scrollbar)),
		guiType(GuiType::ScrollBar) {}

	BaseGui::BaseGui(GuiScrollArea scrollarea) :
		guiBaseName(config.scrollareas.at(scrollarea)),
		guiType(GuiType::ScrollArea) {}

}
