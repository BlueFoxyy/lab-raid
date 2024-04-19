#include <renderer.h>
#include <object/object.h>
#include <config.h>
#include <texture_handler.h>
#include <utility/pointer_wrappers.h>

#include <SDL.h>

#include <cstdlib>
#include <vector>
#include <memory>
#include <set>
#include <format>
#include <string>
#include <exception>

const double PI = acos(-1);

Renderer::Renderer() {
	SDL_Window* windowPtr;
	SDL_Renderer* rendererPtr;
	int returnSuccess = SDL_CreateWindowAndRenderer(
		Config::screenWidth,
		Config::screenHeight,
		Config::screenType,
		&windowPtr,
		&rendererPtr
	);
	if (returnSuccess < 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, std::format(
			"Window & Renderer could not be created. SDL_GetError(): {}",
			SDL_GetError()
		).c_str());
		exit(EXIT_FAILURE);
	}
	SDL_SetWindowTitle(windowPtr, Config::gameTitle.c_str());
	SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0xFF, 0xFF);

	window = sdl_unique_ptr<SDL_Window>(windowPtr);
	renderer = sdl_unique_ptr<SDL_Renderer>(rendererPtr);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized Renderer.");
}

Renderer& Renderer::getInstance(void) {
	static Renderer gRenderer;
	return gRenderer;
}

SDL_Texture* Renderer::createTexture(SDL_Surface* surface) const {
	SDL_Texture* createdTexture = SDL_CreateTextureFromSurface(renderer.get(), surface);
	if (createdTexture == nullptr)
		throw std::logic_error(std::format(
			"Failed to create texture. SDL_GetError(): {}", SDL_GetError()
		));
	return createdTexture;
}

bool Renderer::registerObject(const std::shared_ptr<Objects::Object>& objectPtr) noexcept {
	if (objectPtr.get() == nullptr) return false;
	auto it = objects.find(objectPtr);
	if (it != objects.end()) return false;
	objects.insert(objectPtr);
	TextureHandler::getInstance().loadTexture(*objectPtr);
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Renderer: registered object at %p.", objectPtr.get());

	return true;
}

bool Renderer::removeObject(const std::shared_ptr<Objects::Object>& objectPtr) noexcept {
	if (objectPtr.get() == nullptr) return false;
	auto it = objects.find(objectPtr);
	if (it == objects.end()) return false;
	TextureHandler::getInstance().unloadTexture(*objectPtr);
	objects.erase(it);
	return true;
}

void Renderer::render() {
	static int beginTick, endTick;
	beginTick = SDL_GetTicks();

	if (SDL_RenderClear(renderer.get()) < 0) {
		throw std::logic_error(std::format(
			"Error while clearing render viewport. "
			"SDL_GetError(): {}", SDL_GetError()
		));
	}
	int renderSuccess;
	std::vector<std::weak_ptr<Objects::Object>> deleteVec;
	for (auto& objectWeakPtr : objects) {
		if (auto objectPtr = objectWeakPtr.lock()) {
//			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Rendering object: "); // DEBUG
//			objectPtr->debug(); // DEBUG
			auto rect = objectPtr->getRenderRect();
//			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Target Rect: {%lf, %lf, %lf, %lf}",
//				rect->x, rect->y, rect->w, rect->h
//			); // DEBUG
			SDL_SetTextureAlphaMod(objectPtr->getTexture(), objectPtr->getAlpha());
			renderSuccess = SDL_RenderCopyExF(
				renderer.get(),
				objectPtr->getTexture(),
				NULL,
				objectPtr->getRenderRect(),
				- objectPtr->getAngle() * 180 / PI,
				NULL,
				objectPtr->getFlipFlag()
			);
			if (renderSuccess < 0) {
				throw std::logic_error(std::format(
					"Error while rendering object. "
					"SDL_GetError(): {}", SDL_GetError()
				));
			}
		} else {
			deleteVec.push_back(objectWeakPtr);
		}
	}
	for (auto& objectWeakPtr : deleteVec) {
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,
			"Renderer: removed deleted object."
		);
		objects.erase(objectWeakPtr);
	}
	SDL_RenderPresent(renderer.get());

	endTick = SDL_GetTicks();
	SDL_LogVerbose(
		SDL_LOG_CATEGORY_APPLICATION,
		"Rendered frame in %d ticks.",
		endTick - beginTick
	);

	//debug(); // DEBUG
}

void Renderer::clear() {
	objects.clear();
	TextureHandler::getInstance().clear();
}

void Renderer::debug() const noexcept {
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Registered objects in Renderer:");
	for (auto& objectPtr : objects) {
		if (auto tmp = objectPtr.lock()) {
			tmp->debug();
		} else {
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[deleted object]");
		}
	}
}