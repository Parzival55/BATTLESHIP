#include "ASGEGame.hpp"
#include <Engine/Logger.hpp>

int main()
{
	ASGE::GameSettings game_settings;
	game_settings.window_title = "[Client] ASGENetGame";
	game_settings.window_width = 1024;
	game_settings.window_height = 768;
	game_settings.mode = ASGE::GameSettings::WindowMode::WINDOWED;
	game_settings.fixed_ts = 30;
	game_settings.fps_limit = 120;
	game_settings.msaa_level = 16;
	
	Logging::INFO("Launching Game!");
	ASGENetGame game(game_settings);
	game.run();
	return 0;
}
