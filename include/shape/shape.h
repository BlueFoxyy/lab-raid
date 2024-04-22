#pragma once

#include <view/view.h>
#include <renderer.h>
#include <SDL2/SDL.h>

namespace Views {
	class View;
}

namespace Shapes {
	class Shape {
		friend class Renderer;
	private:
		/// <summary>
		/// Calls draw function after transforming coordinates with view.
		/// </summary>
		/// <param name="view">The target view port.</param>
		virtual void draw(SDL_Renderer* renderer, const Views::View* view) const noexcept = 0;
	protected:
		SDL_Color color;
	public:
		Shape(const SDL_Color& color = { 0, 0, 0, 255 });
		virtual ~Shape() {}

		void setColor(const SDL_Color& newColor) noexcept;
		SDL_Color getColor(void) const noexcept;
	};
}