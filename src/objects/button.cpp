#include <object/button.h>
#include <view/view.h>

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
			{ "button-1", "button-2", "button-3" },
			view,
			position,
			dimension
		), text(text), actionFunc(action), hover(false) {
		Object::setColorMask(color);
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
		Object::setTexture("button-3");
		actionFunc();
	}

	void Button::update(void) noexcept {
		Object::update();
		if (pollHover()) {
			Object::setTexture("button-2");
		} else {
			Object::setTexture("button-1");
		}
	}
}