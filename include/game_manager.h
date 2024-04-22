#pragma once

#include <scene/scene.h>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class GameManager {
private:
	bool paused;
	enum {
		GAME_TITLE = 1,
		GAME_LEVEL = 2,
		GAME_END = 3
	} state;

	std::vector<Scenes::Scene> scenes;
};