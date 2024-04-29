#pragma once

#include <render_object_base.h>
#include <object/object.h>
#include <utility/pointer_wrappers.h>
#include <texture/texture_handler.h>
#include <shape/shape.h>
#include <SDL2/SDL.h>
#include <memory>
#include <set>
#include <stdexcept>
#include <format>

namespace Objects {
	class Object;
}

// Singleton is needed as the renderer can only be initialized at runtime.
/**
 * This is a global singleton class for rendering.
 * Keeps track of current objects, shapes and renders everything onto a set window.
 */
class Renderer {
	/// <summary>
	/// Required key to use renderer to create a texture.
	/// </summary>
	class CreateTextureKey {
		friend class TextureHandler;
	private:
		CreateTextureKey() = default;
		CreateTextureKey(const CreateTextureKey&) = default;
	};

public: // TODO: change this to private, this is for testing purposes.
	/// <summary>
	/// Required key to call render() in @class Renderer.
	/// </summary>
	class RenderKey {
	public:	// TODO: change this to private, this is for testing purposes.
		RenderKey() = default;
		RenderKey(const RenderKey&) = default;
	};

private:
	sdl_unique_ptr<SDL_Window> window;
	sdl_unique_ptr<SDL_Renderer> renderer;
	std::set<std::weak_ptr<RenderObjectBase>, std::owner_less<std::weak_ptr<RenderObjectBase>>> objects;

	/// <summary>
	/// Creates window and renderer.
	/// Declared in private for singleton pattern
	/// </summary>
	Renderer();
public:
	/* SINGLETON PATTERN */
	Renderer(const Renderer&) = delete;
	void operator = (const Renderer&) = delete;
	static Renderer& getInstance(void) noexcept;
	/* SINGLETON PATTERN */

	/// <summary>
	/// Creates a texture from a SDL_Surface.
	/// </summary>
	/// <param name="key">Required key to use this function.</param>
	/// <param name="surface">The source surface.</param>
	/// <returns>A pointer to the allocated SDL_Texture object.</returns>
	SDL_Texture* createTexture(CreateTextureKey key, SDL_Surface* surface) const;

	/// <summary>
	/// Get underlying SDL_Renderer renderer.
	/// </summary>
	/// <returns>The underlying renderer.</returns>
	//SDL_Renderer* getRenderer(void) noexcept;

	/// <summary>
	/// Registers the object for rendering.
	/// </summary>
	/// <param name="objectPtr">std::shared_ptr of the object</param>
	/// <returns>Whether the object was successfully registered</returns>
	bool registerObject(std::shared_ptr<RenderObjectBase> objectPtr) noexcept;

	/// <summary>
	/// Unregisters the object for rendering.
	/// </summary>
	/// <param name="objectPtr">std::shared_ptr of the object</param>
	/// <returns>Whether the object was successfully unregistered.</returns>
	bool removeObject(std::shared_ptr<RenderObjectBase> objectPtr) noexcept;

	/// <summary>
	/// Renders every registered object.
	/// Note: SDL has built-in out of boundaries check.
	/// </summary>
	/// <param name="key">Access Control Key</param>
	void render(RenderKey key);

	/// <summary>
	/// Clears object set and unloads all textures.
	/// </summary>
	void clear() noexcept;
	
	/// <summary>
	/// Prints renderer debug info.
	/// </summary>
	void debug(void) const noexcept;
};