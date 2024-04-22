#pragma once

#include "texture_info.h"
#include <utility/pointer_wrappers.h>
#include <object/object.h>
#include <SDL2/SDL.h>
#include <string>
#include <map>
#include <memory>

namespace Objects {
	class Object;
}

/// <summary>
/// This is a global singleton class for texture handling.
/// </summary>
class TextureHandler {
	friend class Renderer;
private:
	static const std::string errorTextureName;
	std::map<std::string, std::shared_ptr<SDL_Texture>> textureDB;

	/// <summary>
	/// Loads error texture.
	/// </summary>
	TextureHandler();

	void loadTexture(const std::string& textureName);

	/// <summary>
	/// Loads texture for the input object.
	/// </summary>
	/// <param name="object">The object to be loaded</param>
	void loadTexture(Objects::Object& object);

	/// <summary>
	/// Unloads texture for the input object.
	/// </summary>
	/// <param name="object">The object to be unloaded</param>
	void unloadTexture(Objects::Object& object);

	/// <summary>
	/// Reloads texture for the input object.
	/// </summary>
	/// <param name="object">The object to be reloaded</param>
	void reloadTexture(Objects::Object& object);

	/// <summary>
	/// Unloads every texture except for 'error'
	/// </summary>
	void clear(void);
public:
	TextureHandler(const TextureHandler&) = delete;
	void operator = (const TextureHandler&) = delete;
	static TextureHandler& getInstance(void);
};