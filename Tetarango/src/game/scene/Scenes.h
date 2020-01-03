#pragma once

#include <string>
#include <map>

namespace AWGame
{
	enum class SceneGame
	{
		None,
		Splash,
		MainMenu,
		OptionsMenu,
		SavedGamesMenu,

		// Test
		Tetris,

		// Game
		MainGame,
		WorldTetarango
	};

	class Scenes
	{
	public:
		const std::map<SceneGame, std::string> game
		{
			{ SceneGame::Splash, "menu-splash" },
			{ SceneGame::MainMenu, "menu-main" },
			{ SceneGame::OptionsMenu, "menu-options" },
			{ SceneGame::SavedGamesMenu, "menu-saved-games" },

			{ SceneGame::Tetris, "game-tetris" },

			{ SceneGame::MainGame, "game-main" },
			{ SceneGame::WorldTetarango, "world-tetarango"}
		};
	};
}

