#include <managers/tick_manager.h>
#include <SDL2/SDL_timer.h>

void TickManager::init() {
	uint32_t initialTick = SDL_GetTicks();
	lastTick = currentTick = initialTick;
	diffTick = 0;
}

void TickManager::update() {
	uint32_t newTick = SDL_GetTicks();
	lastTick = currentTick;
	diffTick = newTick - currentTick;
	currentTick = newTick;
}

uint32_t TickManager::getLastTick(void) const {
	return lastTick;
}

uint32_t TickManager::getCurrentTick(void) const {
	return currentTick;
}

uint32_t TickManager::getDiffTick(void) const {
	return diffTick;
}