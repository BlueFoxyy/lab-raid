#pragma once

#include <object/object.h>
#include <view/views.h>
#include <renderer.h>
#include <config.h>
#include <utility/vector2d.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <memory>
#include <string>
#include <vector>

namespace Global {
	std::unique_ptr<FPSmanager> fpsManager;

	std::unique_ptr<Views::Camera> playerCamera;
	std::unique_ptr<Views::HUD> hudView;
	std::unique_ptr<Views::HUD> menuView;

	std::shared_ptr<Objects::Object> playerObject, arrowObject;

	void init();
}