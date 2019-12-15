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

		// Game
		MainGame,
		Tetris
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
			{ SceneGame::Tetris, "game-tetris" }
		};
	};
}

