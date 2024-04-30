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
	extern std::shared_ptr<Shapes::Circle> yellowCircle;
	extern std::shared_ptr<Shapes::Circle> greenCircle;
	extern std::shared_ptr<Shapes::Circle> blueCircle;
	extern std::shared_ptr<Shapes::Circle> redCircle;
	extern std::shared_ptr<Shapes::Circle> purpleCircle;

	extern std::shared_ptr<Shapes::HollowCircle> hollowCircle1;
	extern std::shared_ptr<Shapes::Line> line1;
	extern std::shared_ptr<Shapes::Line> line2;
	extern std::shared_ptr<Shapes::Line> line3;
	extern std::shared_ptr<Shapes::Line> line4;

	extern std::shared_ptr<Shapes::Line> crosshairLine1;
	extern std::shared_ptr<Shapes::Line> crosshairLine2;
	extern std::shared_ptr<Shapes::HollowCircle> crosshairCircle1;

	void init();
}