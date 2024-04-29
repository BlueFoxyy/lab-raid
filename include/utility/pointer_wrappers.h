#pragma once

#include <SDL2/SDL.h>
#include <memory>

/** \brief Generic deleter functor for SDL resources.
 * For use with std smart pointers.
 */
struct sdl_deleter {
	inline void operator () (SDL_RWops* thing) const noexcept		{ if (thing) SDL_FreeRW(thing); }
	inline void operator () (SDL_cond* thing) const noexcept		{ if (thing) SDL_DestroyCond(thing); }
	inline void operator () (SDL_Cursor* thing) const noexcept		{ if (thing) SDL_FreeCursor(thing); }
	inline void operator () (SDL_PixelFormat* thing) const noexcept { if (thing) SDL_FreeFormat(thing); }
	inline void operator () (SDL_mutex* thing) const noexcept		{ if (thing) SDL_DestroyMutex(thing); }
	inline void operator () (SDL_Palette* thing) const noexcept		{ if (thing) SDL_FreePalette(thing); }
	inline void operator () (SDL_Renderer* thing) const noexcept	{ if (thing) SDL_DestroyRenderer(thing); }
	inline void operator () (SDL_sem* thing) const noexcept			{ if (thing) SDL_DestroySemaphore(thing); }
	inline void operator () (SDL_Surface* thing) const noexcept		{ if (thing) SDL_FreeSurface(thing); }
	inline void operator () (SDL_Texture* thing) const noexcept		{ if (thing) SDL_DestroyTexture(thing); }
	inline void operator () (Uint8* thing) const noexcept			{ if (thing) SDL_FreeWAV(thing); }
	inline void operator () (SDL_Window* thing) const noexcept		{ if (thing) SDL_DestroyWindow(thing); }
};

template <typename Resource>
using sdl_unique_ptr = std::unique_ptr<Resource, sdl_deleter>;

template <typename Resource>
std::shared_ptr<Resource> sdl_make_shared(Resource* resource) {
	return std::shared_ptr<Resource>(resource, sdl_deleter());
}