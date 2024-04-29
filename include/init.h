#pragma once

#include <object/object.h>
#include <view/views.h>
#include <renderer.h>
#include <config.h>
#include <utility/vector2d.h>
#include <shape/shapes.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <memory>
#include <string>
#include <vector>

namespace Global {
	extern std::unique_ptr<FPSmanager> fpsManager;

	extern std::unique_ptr<Views::Camera> playerCamera;
	extern std::unique_ptr<Views::HUD> hudView;
	extern std::unique_ptr<Views::HUD> menuView;

	extern std::shared_ptr<Objects::Object> playerObject, arrowObject1;
	extern std::shared_ptr<Objects::Object> arrowObject2;

	void init();
}