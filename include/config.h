#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace Config {
	const std::string gameTitle = "Lab Raid";
	const int screenWidth = 1280;
	const int screenHeight = 768;
	const int volume = 50;
	const int framerate = 60;
	const float holdTimeThreshold = 100;
	const SDL_WindowFlags screenType = SDL_WINDOW_SHOWN;
	//const SDL_Color backgroundColor{ 0x1F, 0x1E, 0x33, 0x7F };
	const SDL_Color backgroundColor{ 0x3F, 0x3F, 0x3F, 0xFF };
}