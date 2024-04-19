#pragma once

#include "command.h"
#include <SDL.h>
#include <functional>
#include <map>
#include <memory>

/// <summary>
/// This is a global singleton class of handling user inputs.
/// Wrapper class of SDL_PollEvent and events handling.
/// </summary>
class InputHandler {
private:
	std::map<SDL_KeyCode, std::unique_ptr<Commands::Command>> commandDB;
public:


	
};