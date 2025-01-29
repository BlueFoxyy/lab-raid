#pragma once

#include <cstdint>
#include <memory>

class TickManager {
private:
	uint32_t lastTick;
	uint32_t currentTick;
	uint32_t diffTick;
public:
	void init(void);
	void update(void);
	uint32_t getLastTick(void) const;
	uint32_t getCurrentTick(void) const;
	uint32_t getDiffTick(void) const;
};