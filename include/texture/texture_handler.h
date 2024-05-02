#pragma once

#include <renderer.h>
#include <utility/pointer_wrappers.h>
#include <object/object.h>
#include <SDL2/SDL.h>
#include <string>
#include <map>
#include <memory>
#include <format>

namespace Objects {
	class Object;
}

// TODO: Add support for text textures.

/// <summary>
/// This is a global singleton class for texture handling.
/// </summary>
class TextureHandler {
	/// <summary>
	/// Required key to request texture from @class TextureHandler.
	/// </summary>
	class TextureRequestKey {
		friend class Objects::Object;
	private:
		TextureRequestKey() = default;
		TextureRequestKey(const TextureRequestKey&) = default;
	};

private:
	static const std::string errorTextureName;
	std::map<std::string, sdl_unique_ptr<SDL_Texture>> textureDB;

	/// <summary>
	/// Loads error texture.
	/// </summary>
	TextureHandler();
	
	void loadTexture(const std::string& textureName);

public:
	/// <summary>
	/// Gets a weak pointer pointing to the requested texture.
	/// </summary>
	/// <param name="key">Access Control Key</param>
	/// <param name="textureName">The name of the texture.</param>
	/// <returns>The raw pointer of the requested texture.</returns>
	SDL_Texture* getTexture(TextureRequestKey key, const std::string& textureName);

public:
	TextureHandler(const TextureHandler&) = delete;
	void operator = (const TextureHandler&) = delete;
	static TextureHandler& getInstance(void);
};