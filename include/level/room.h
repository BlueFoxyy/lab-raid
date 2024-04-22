#pragma once

#include "level.h"
#include <object/object.h>
#include <utility/vector2d.h>
#include <geometry/line.h>
#include <SDL2/SDL.h>
#include <vector>
#include <fstream>

namespace Rooms {
	const std::string roomDir = "resources/rooms/";

	class Room {
		friend class Level;
	private:
		Vector2D position;
		Vector2D dimension;

		std::vector<Line> walls;

		void loadFromFile(const std::string& roomName);
	public:
		virtual ~Room() = 0;

	};
}