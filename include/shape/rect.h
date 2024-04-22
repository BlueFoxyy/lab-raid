#pragma once

#include <shape/shape.h>

namespace Shapes {
	class Rect : public Shape {
	private:
		//void draw()
	protected:
		Vector2D position;
		Vector2D dimension;
	};
}