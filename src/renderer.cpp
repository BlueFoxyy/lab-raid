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

SDL_Window* Renderer::getWindow(void) noexcept {
	return window.get();
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
	auto it = objectListMap.find(objectPtr);
	if (it != objectListMap.end()) return false;
	objectList.push_back(objectPtr);
	objectListMap[objectPtr] = --objectList.end();
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,
		"Renderer: registered object at %p with list node at %p.",
		objectPtr.get(),
		objectListMap.at(objectPtr)
	);

	/*
	if (std::dynamic_pointer_cast<Objects::Object>(objectPtr)) {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderer: registered object type is Object.");
	} else if (std::dynamic_pointer_cast<Shapes::Shape>(objectPtr)) {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderer: registered object type is Shape.");
	}
	*/

	return true;
}

bool Renderer::removeObject(std::weak_ptr<RenderObjectBase> objectPtr) noexcept {
	auto it = objectListMap.find(objectPtr);
	if (it == objectListMap.end()) return false;
	objectList.erase(it->second);
	objectListMap.erase(it);

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Renderer: unregistered object.");

	return true;
}

void Renderer::render(const RenderKey& key) {
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
	for (auto& objectWeakPtr : objectList) {
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
					-objectPtr->getRenderAngle() * 180 / PI,
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
		//system("pause");
		removeObject(objectWeakPtr);
	}

	// TODO: Draw temporary objects here.

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

void Renderer::moveLayerUp(std::shared_ptr<RenderObjectBase> objectPtr) {
	auto it = objectListMap.find(objectPtr);
	if (it == objectListMap.end())
		throw std::invalid_argument("In Renderer::moveLayerUp(): object is not registered.");
	auto curListIt = it->second;
	if (curListIt == --objectList.end())
		return;
	auto nextListIt = curListIt; nextListIt++;
	auto nextObjectPtr = *nextListIt;
	swap(objectListMap[objectPtr], objectListMap[nextObjectPtr]);
	swap(*curListIt, *nextListIt);
}

void Renderer::moveLayerDown(std::shared_ptr<RenderObjectBase> objectPtr) {
	auto it = objectListMap.find(objectPtr);
	if (it == objectListMap.end())
		throw std::invalid_argument("In Renderer::moveLayerDown(): object is not registered.");
	auto curListIt = it->second;
	if (curListIt == objectList.begin())
		return;
	auto prevListIt = curListIt; prevListIt--;
	auto prevObjectPtr = *prevListIt;
	swap(objectListMap[objectPtr], objectListMap[prevObjectPtr]);
	swap(*curListIt, *prevListIt);
}

void Renderer::moveLayerTop(std::shared_ptr<RenderObjectBase> objectPtr) {
	auto it = objectListMap.find(objectPtr);
	if (it == objectListMap.end())
		throw std::invalid_argument("In Renderer::moveLayerTop(): object is not registered.");
	objectList.erase(it->second);
	objectList.push_back(objectPtr);
	it->second = --objectList.end();
}

void Renderer::moveLayerBottom(std::shared_ptr<RenderObjectBase> objectPtr) {
	auto it = objectListMap.find(objectPtr);
	if (it == objectListMap.end())
		throw std::invalid_argument("In Renderer::moveLayerBottom(): object is not registered.");
	objectList.erase(it->second);
	objectList.push_front(objectPtr);
	it->second = objectList.begin();
}

void Renderer::clear() noexcept {
	objectList.clear();
	objectListMap.clear();
}

void Renderer::debug() const noexcept {
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Registered objects in Renderer:");
	for (auto& objectPtr : objectList) {
		if (auto tmp = objectPtr.lock()) {
			tmp->debug();
		} else {
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[deleted object]");
		}
	}
}