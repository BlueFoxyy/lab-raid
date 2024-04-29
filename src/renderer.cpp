#include <renderer.h>

const float PI = acosf(-1);

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
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"Window & Renderer could not be created. SDL_GetError(): %s",
			SDL_GetError()
		);
		exit(EXIT_FAILURE);
	}
	SDL_SetWindowTitle(windowPtr, Config::gameTitle.c_str());
	SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0xFF, 0xFF);

	window = sdl_unique_ptr<SDL_Window>(windowPtr);
	renderer = sdl_unique_ptr<SDL_Renderer>(rendererPtr);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized Renderer.");
}

Renderer& Renderer::getInstance(void) noexcept {
	static Renderer gRenderer;
	return gRenderer;
}

/*
SDL_Renderer* Renderer::getRenderer(void) noexcept {
	return renderer.get();
}
*/

SDL_Texture* Renderer::createTexture(CreateTextureKey key, SDL_Surface* surface) const {
	SDL_Texture* createdTexture = SDL_CreateTextureFromSurface(renderer.get(), surface);
	if (createdTexture == nullptr)
		throw std::logic_error(std::format(
			"Failed to create texture. SDL_GetError(): {}", SDL_GetError()
		));
	return createdTexture;
}

bool Renderer::registerObject(std::shared_ptr<RenderObjectBase> objectPtr) noexcept {
	if (objectPtr.get() == nullptr) return false;
	auto it = objects.find(objectPtr);
	if (it != objects.end()) return false;
	objects.insert(objectPtr);
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Renderer: registered object at %p.", objectPtr.get());
	if (std::dynamic_pointer_cast<Objects::Object>(objectPtr)) {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderer: registered object type is Object.");
	} else if (std::dynamic_pointer_cast<Shapes::Shape>(objectPtr)) {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderer: registered object type is Shape.");
	}

	return true;
}

bool Renderer::removeObject(std::shared_ptr<RenderObjectBase> objectPtr) noexcept {
	if (objectPtr.get() == nullptr) return false;
	auto it = objects.find(objectPtr);
	if (it == objects.end()) return false;
	objects.erase(it);

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Renderer: unregistered object at %p.", objectPtr.get());

	return true;
}

void Renderer::render(RenderKey key) {
	static int beginTick, endTick;
	beginTick = SDL_GetTicks();

	if (SDL_RenderClear(renderer.get()) < 0) {
		throw std::logic_error(std::format(
			"Error while clearing render viewport. "
			"SDL_GetError(): {}", SDL_GetError()
		));
	}

	int renderSuccess;
	std::vector<std::weak_ptr<RenderObjectBase>> deleteVec;
	for (auto& objectWeakPtr : objects) {
		if (auto objectSharedPtr = objectWeakPtr.lock()) {
//			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
//				"Rendering object at %p.",
//				objectSharedPtr.get()
//			);
			if (auto objectPtr = std::dynamic_pointer_cast<Objects::Object>(objectSharedPtr)) {
				if (not objectPtr->getVisibility()) continue;
				//			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Rendering object: "); // DEBUG
				//			objectPtr->debug(); // DEBUG
				auto rect = objectPtr->getRenderRect();
				//			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Target Rect: {%lf, %lf, %lf, %lf}",
				//				rect->x, rect->y, rect->w, rect->h
				//			); // DEBUG
				renderSuccess = SDL_RenderCopyExF(
					renderer.get(),
					objectPtr->getTexture(),
					NULL,
					std::make_unique<SDL_FRect>(objectPtr->getRenderRect()).get(),
					-objectPtr->getAngle() * 180 / PI,
					NULL,
					objectPtr->getFlipFlag()
				);
				if (renderSuccess < 0) {
					throw std::logic_error(std::format(
						"Error while rendering object. "
						"SDL_GetError(): {}", SDL_GetError()
					));
				}
			} else if (auto shapePtr = std::dynamic_pointer_cast<Shapes::Shape>(objectSharedPtr)) {
//				SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
//					"Renderer::render(): Rendering shape."
//				);
				shapePtr->draw(renderer.get());
			}
		} else {
			deleteVec.push_back(objectWeakPtr);
		}
	}
	for (auto& objectWeakPtr : deleteVec) {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
			"Renderer: removed deleted object."
		);
		system("pause");
		objects.erase(objectWeakPtr);
	}

	SDL_SetRenderDrawColor(
		renderer.get(),
		Config::backgroundColor.r,
		Config::backgroundColor.g,
		Config::backgroundColor.b,
		Config::backgroundColor.a
	);

	SDL_RenderPresent(renderer.get());

	endTick = SDL_GetTicks();
	SDL_LogVerbose(
		SDL_LOG_CATEGORY_APPLICATION,
		"Rendered frame in %d ticks.",
		endTick - beginTick
	);

	//debug(); // DEBUG
}

void Renderer::clear() noexcept {
	objects.clear();
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