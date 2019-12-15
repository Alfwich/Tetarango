#pragma once

#include <memory>
#include "engine/module/SystemModuleBundle.h"
#include "Guis.h"

namespace MTGame
{
	class BaseGui
	{
		enum class GuiType
		{
			Button,
			Camera,
			Menu,
			Transition,
			Hud,
			ScrollBar,
			ScrollArea
		};

	protected:
		static const Guis& getGuis();

		const GuiType guiType;
		const std::string guiBaseName;

	public:
		static void primeGuis();

		BaseGui(GuiButton button);
		BaseGui(GuiCamera camera);
		BaseGui(GuiMenu menu);
		BaseGui(GuiTransition transition);
		BaseGui(GuiHud hud);
		BaseGui(GuiScrollBar scrollBar);
		BaseGui(GuiScrollArea scrollArea);

		std::weak_ptr<MT::ApplicationObject> clickListener;
		std::weak_ptr<MT::ApplicationObject> scrollListener;
	};

}

