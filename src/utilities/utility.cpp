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