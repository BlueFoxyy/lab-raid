#pragma once

#include <level/level.h>
#include <view/views.h>
#include <SDL.h>
#include <vector>
#include <memory>

class Game {
private:
	bool paused;
	enum {
		GAME_TITLE = 1,
		GAME_LEVEL = 2,
		GAME_END = 3
	} state;

	std::vector<std::unique_ptr<Levels::Level>> levels;
	
	std::unique_ptr<Views::Camera> camera;
	std::unique_ptr<Views::HUD> hud;
};