#pragma once

#include <SDL2/SDL.h>
#include <memory>

/** \brief Generic deleter functor for SDL resources.
 * For use with std smart pointers.
 */
struct sdl_deleter {
	// Generate "free" calls.
# define FREE_ME(type, fn_name) inline void operator()(type* thing)  \
	const noexcept { if (thing) SDL_##fn_name(thing); }
	FREE_ME(SDL_RWops, FreeRW)			 FREE_ME(SDL_cond, DestroyCond)
		FREE_ME(SDL_Cursor, FreeCursor)		FREE_ME(SDL_PixelFormat, FreeFormat)
		FREE_ME(SDL_mutex, DestroyMutex)	   FREE_ME(SDL_Palette, FreePalette)
		FREE_ME(SDL_Renderer, DestroyRenderer) FREE_ME(SDL_sem, DestroySemaphore)
		FREE_ME(SDL_Surface, FreeSurface)	  FREE_ME(SDL_Texture, DestroyTexture)
		FREE_ME(Uint8, FreeWAV)				FREE_ME(SDL_Window, DestroyWindow)
#undef FREE_ME
};

template <typename Resource>
using sdl_unique_ptr = std::unique_ptr<Resource, sdl_deleter>;

template <typename Resource>
std::shared_ptr<Resource> sdl_make_shared(Resource* resource) {
	return std::shared_ptr<Resource>(resource, sdl_deleter());
}