#include <texture/texture_handler.h>

const std::string fileExtension = ".bmp";

static const char* getResource(const std::string& resourceName) {
	static const std::string dirname = "resources/textures/";
	static std::string pathname;
	pathname = dirname + resourceName + fileExtension;
	return pathname.c_str();
}

const std::string TextureHandler::errorTextureName = "error";

void TextureHandler::loadTexture(const std::string& textureName) {
	SDL_Surface* surface;
	std::string textureFile = getResource(textureName);
	surface = SDL_LoadBMP(textureFile.c_str());
	if (surface == nullptr)
		throw std::logic_error(std::format(
			"Cannot load '{}'. SDL_GetError(): {}", textureName, SDL_GetError()
		));
	SDL_Texture* texture;
	try {
		texture = Renderer::getInstance().createTexture({}, surface);
	} catch (std::exception& e) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, e.what());
		throw std::logic_error(std::format(
			"Cannot create texture for '{}'", textureFile
		));
	}

	textureDB[textureName] = sdl_unique_ptr<SDL_Texture>(texture);
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, std::format(
		"Loaded texture '{}'", textureFile
	).c_str());
}

TextureHandler::TextureHandler() {
	static const int size = 8;
	SDL_Surface* errorSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, size, size, 32, 0, 0, 0, 0);
	if (errorSurface == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, std::format(
			"Failed to create 'error' surface. SDL_GetError: {}", SDL_GetError()
		).c_str());
		exit(EXIT_FAILURE);
	}

	int h = size / 2; // half
	SDL_Rect ULRect{ 0, 0, h, h };
	SDL_Rect URRect{ h, 0, h, h };
	SDL_Rect DLRect{ 0, h, h, h };
	SDL_Rect DRRect{ h, h, h, h };
	SDL_FillRect(errorSurface, &ULRect, SDL_MapRGB(errorSurface->format, 255, 0, 220));
	SDL_FillRect(errorSurface, &URRect, SDL_MapRGB(errorSurface->format, 0, 0, 0));
	SDL_FillRect(errorSurface, &DLRect, SDL_MapRGB(errorSurface->format, 0, 0, 0));
	SDL_FillRect(errorSurface, &DRRect, SDL_MapRGB(errorSurface->format, 255, 0, 220));

	SDL_Texture* errorTexture;
	try {
		errorTexture = Renderer::getInstance().createTexture({}, errorSurface);
	} catch (std::exception& e) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, std::format(
			"Failed to create 'error' texture. what(): {}", e.what()
		).c_str());
		exit(EXIT_FAILURE);
	}
	textureDB[errorTextureName] = sdl_unique_ptr<SDL_Texture>(errorTexture);
}

TextureHandler& TextureHandler::getInstance(void) {
	static TextureHandler textureHandler;
	return textureHandler;
}

SDL_Texture* TextureHandler::getTexture(TextureRequestKey key, const std::string& textureName) {
	if (textureDB.find(textureName) == textureDB.end())
		loadTexture(textureName);
	return textureDB.at(textureName).get();
}