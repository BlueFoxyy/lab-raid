#include <shape/shape.h>

namespace Shapes {
	Shape::Shape(Views::View* view, const SDL_Color& color):
		color(color), view(view) {}
	void Shape::setColor(const SDL_Color& newColor) noexcept {
		color = newColor;
	}
	SDL_Color Shape::getColor(void) const noexcept {
		return color;
	}
}