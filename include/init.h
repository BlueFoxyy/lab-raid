#pragma once

#include <SDL.h>
#include <SDL2_framerate.h>
#include <view/views.h>
#include <memory>

namespace Global {
	std::unique_ptr<FPSmanager> fpsManager;

	std::unique_ptr<Views::Camera> playerCamera;
	std::unique_ptr<Views::HUD> hudView;
	std::unique_ptr<Views::HUD> menuView;

	std::shared_ptr<Objects::Object> playerObject, object;

	void init();
}