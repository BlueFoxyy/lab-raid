#pragma once

#include <init.h>
#include <object/object.h>
#include <object/obstacle/obstacle.h>

namespace Obstacles {
	class WoodenCrate : public Object, public Obstacle {
	public:
		WoodenCrate():
			Object()
	};
};