#include "BaseGui.h"

#include <string>
#include "GameImports.h"
#include "Guis.h"

namespace
{
	const std::string buttonBasicId = "button-basic";
	const std::string mediumFontId = "medium";
	const MTGame::Guis config;
}

namespace MTGame
{
	bool BaseGui::isLoaded = false;

	const Guis& BaseGui::getGuis()
	{
		return config;
	}

	void BaseGui::primeGuis()
	{
		std::make_unique<ButtonBasic>();
		std::make_unique<GameCamera>();
		std::make_unique<GameMainMenu>();
		std::make_unique<TransitionFade>();
	}

	void BaseGui::loadResources(std::shared_ptr<MT::SystemModuleBundle> modules)
	{
		if (isLoaded)
		{
			return;
		}

		modules->font->loadFont("res/game/font/Roboto-Medium.ttf", mediumFontId);
		modules->texture->loadTexture("res/game/img/ui/button/proto_button.png", buttonBasicId);
		modules->animation->addAnimationSet(std::make_shared<ButtonBasicAnimation>(), buttonBasicId);

		isLoaded = true;
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
}
