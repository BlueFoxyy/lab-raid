#include <utility/utility.h>

float normalizeAngle(float angle) noexcept {
	if (angle < 0) {
		angle += static_cast<float>(-ceil(angle / (2 * M_PI)) * (2 * M_PI));
	} else {
		angle -= static_cast<float>(floor(angle / (2 * M_PI)) * (2 * M_PI));
	}
	return angle;
}

Vector2D polarToCartesian(float radius, float theta) {
	return Vector2D{ radius * cosf(theta), radius * sinf(theta) };
}

bool rectCollide(
	const SDL_FRect& rect1, float angle1,
	const SDL_FRect& rect2, float angle2
) {
	std::array<Vector2D, 4> points;
	std::array<const SDL_FRect*, 2> rects;
	std::array<double, 2> angles;

	rects = { &rect1, &rect2 };
	angles = { angle1, angle2 };
	for (int iteration = 1; iteration <= 2; iteration++) {
		Vector2D pivot1(rects[0]->x, rects[0]->y);
		Vector2D pivot2(rects[1]->x, rects[1]->y);
		points[0] = pivot1 - pivot2;
		points[1] = points[0] + Vector2D(rects[0]->w, 0).rotate(angles[0]);
		points[2] = points[0] + Vector2D(0, rects[0]->h).rotate(angles[0]);
		points[3] = points[0] + Vector2D(rects[0]->w, rects[0]->h).rotate(angles[0]);

		for (auto& point : points)
			if (0 <= point.getX() and point.getX() <= rects[1]->w
			and 0 <= point.getY() and point.getY() <= rects[1]->h)
				return true;

		std::swap(rects[0], rects[1]);
		std::swap(angles[0], angles[1]);
	}
	
	return false;
}