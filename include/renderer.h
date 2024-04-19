#pragma once

#include <object/object.h>
#include <utility/pointer_wrappers.h>

#include <SDL.h>
#include <memory>
#include <set>

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

/**
 * This is a global singleton class for rendering.
 * Keeps track of current objects and renders everything onto a set window.
 */
class Renderer {
	friend TextureHandler;

private:
	sdl_unique_ptr<SDL_Window> window;
	sdl_unique_ptr<SDL_Renderer> renderer;
	std::set<std::weak_ptr<Objects::Object>, std::owner_less<std::weak_ptr<Objects::Object>>> objects;

	SDL_Texture* createTexture(SDL_Surface* surface) const;

	/// <summary>
	/// Creates window and renderer.
	/// Declared in private for singleton pattern
	/// </summary>
	Renderer();
public:
	/* SINGLETON PATTERN */
	Renderer(const Renderer&) = delete;
	void operator = (const Renderer&) = delete;
	static Renderer& getInstance(void);
	/* SINGLETON PATTERN */

	/// <summary>
	/// Registers the object for rendering, and loads texture for the object.
	/// </summary>
	/// <param name="objectPtr">std::shared_ptr of the object</param>
	/// <returns>returns whether the object was successfully registered</returns>
	bool registerObject(const std::shared_ptr<Objects::Object>& objectPtr) noexcept;

	/// <summary>
	/// Unregisters the object for rendering.
	/// </summary>
	/// <param name="objectPtr">std::shared_ptr of the object</param>
	/// <returns>returns whether the object was successfully unregistered.</returns>
	bool removeObject(const std::shared_ptr<Objects::Object>& objectPtr) noexcept;

	/// <summary>
	/// Renders every registered object.
	/// Note: SDL has built-in out of boundaries check.
	/// </summary>
	void render();

	/// <summary>
	/// Clears object set and unloads all textures.
	/// </summary>
	void clear();
	
	/// <summary>
	/// Prints renderer debug info.
	/// </summary>
	void debug(void) const noexcept;
};