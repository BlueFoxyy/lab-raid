#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace Config {
	const std::string gameTitle = "Lab Raid";
	const int screenWidth = 1280;
	const int screenHeight = 768;
	const int volume = 50;
	const int framerate = 60;
	const double holdTimeThreshold = 100;
	const SDL_WindowFlags screenType = SDL_WINDOW_SHOWN;
}