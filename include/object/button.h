#pragma once

#include "object.h"
#include <view/view.h>
#include <string>
#include <functional>

namespace Objects {
	class Button : private Object {
	private:
		std::string text;
		bool hover;
		std::function<void(void)> actionFunc;

		bool pollHover(void) noexcept;

	public:
		Button(
			const Views::View* view,
			const Vector2D& position,
			const Vector2D& dimension,
			const SDL_Color& color,
			const std::string& text,
			std::function<void(void)> action
		);

		void setHovered(void) noexcept;

		void onClick(void) noexcept;

		void update(void) noexcept;
	};
}