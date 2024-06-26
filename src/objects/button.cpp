#include <object/button.h>

namespace Objects {
	Button::Button(
			const Views::View* view,
			const Vector2D& position,
			const Vector2D& dimension,
			const SDL_Color& color,
			const std::string& text,
			std::function<void(void)> action
		) :
		Object(
			{"button-1", "button-2", "button-3"},
			view,
			position,
			dimension
		), text(text), actionFunc(action), hover(false) {
	}

	bool Button::pollHover(void) noexcept {
		if (hover) {
			hover = false;
			return true;
		}
		return false;
	}

	void Button::setHovered(void) noexcept {
		hover = true;
	}

	void Button::onClick(void) noexcept {
		Object::setTexture(2);
		actionFunc();
	}

	void Button::update(void) noexcept {
		Object::update();
		if (pollHover()) {
			Object::setTexture(1);
		} else {
			Object::setTexture(0);
		}
	}
}