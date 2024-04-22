#pragma once

#include <object/object.h>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace Scenes {

	/// <summary>
	/// Represents a game scene (whether it be main menu, level or loading screen).
	/// Basically a object manager. The scenes are managed by the GameManager.
	/// </summary>
	class Scene {
	private:
		std::vector<std::shared_ptr<Objects::Object>> objects;
		
	};
}