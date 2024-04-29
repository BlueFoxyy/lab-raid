#pragma once

#include <render_object_base.h>
#include <view/view.h>
#include <renderer.h>
#include <SDL2/SDL.h>
#include <vector>

namespace Views { class View; }

namespace Shapes {
	class Shape : public RenderObjectBase {
	private:
		/// <summary>
		/// Calls draw function after transforming coordinates with view.
		/// </summary>
		/// <param name="view">The target view port.</param>
	protected:
		const Views::View* view;
		SDL_Color color;
	public:
		virtual void draw(SDL_Renderer* renderer) const noexcept {}
		Shape(Views::View* view, const SDL_Color& color = { 0, 0, 0, 255 });
		virtual ~Shape() {}

		void setColor(const SDL_Color& newColor) noexcept;
		SDL_Color getColor(void) const noexcept;
	};
}