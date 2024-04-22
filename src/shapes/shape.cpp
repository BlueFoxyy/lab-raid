#include <shape/shape.h>

namespace Shapes {
	Shape::Shape(const SDL_Color& color):
		color(color) {}
	void Shape::setColor(const SDL_Color& newColor) noexcept {
		color = newColor;
	}
}