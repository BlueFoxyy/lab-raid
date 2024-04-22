#pragma once

#include "view.h"

namespace Views {
	class HUD : public View {
	public:
		HUD();
		SDL_FRect getRect(const Objects::Object&) const noexcept override;
		Vector2D transform(const Vector2D& position) const noexcept override;
	};
}