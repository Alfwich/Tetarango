#pragma once

#include <string>
#include <map>

namespace MTGame
{
	enum class SceneGame
	{
		None,
		Splash,
		MainMenu,
		OptionsMenu,
		SavedGamesMenu,

		// Game
		MainGame,
		StartFlowShipSelection,
		StartFlowCrewSelection,
		TownOrigin,
		TownOriginShop,

		// Generated
		GeneratedStarSystem,
		GeneratedPlanet
	};

	class Scenes
	{
	public:
		const std::map<SceneGame, std::string> game{
			{ SceneGame::Splash, "menu-splash" },
			{ SceneGame::MainMenu, "menu-main" },
			{ SceneGame::OptionsMenu, "menu-options" },
			{ SceneGame::SavedGamesMenu, "menu-saved-games" },

			{ SceneGame::MainGame, "game-main" },
			{ SceneGame::TownOrigin, "game-town-origin" },
			{ SceneGame::TownOriginShop, "game-town-origin-shop" },
			{ SceneGame::StartFlowShipSelection, "start-flow-ship-selection" },
			{ SceneGame::StartFlowCrewSelection, "start-flow-crew-selection" },

			{ SceneGame::GeneratedStarSystem, "generated-star-system" },
			{ SceneGame::GeneratedPlanet, "generated-planet" },
		};
	};
}

