#pragma once

#include "room.h"
#include <SDL.h>
#include <vector>
#include <memory>
#include <random>

namespace Levels {
	class Level {
	private:
		std::vector<std::unique_ptr<Rooms::Room>> rooms;
		std::mt19937 rnd;

		void generateLevel();
	public:
		Level(unsigned int rndSeed) :
			rnd(rndSeed) {}
	};
};