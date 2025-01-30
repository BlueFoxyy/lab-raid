#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>

#include <managers/tick_manager.h>
#include <managers/command_manager.h>
#include <managers/selection_manager.h>
#include <view/views.h>
#include <object/bullet.h>
#include <shape/shapes.h>

#include <deque>

constexpr uint32_t TICKS_PER_SEC = 1000;

namespace Global {

	extern std::unique_ptr<FPSmanager> fpsManager;
	extern std::unique_ptr<Views::Camera> playerCamera;
	extern std::unique_ptr<Views::HUD> hudView;
	extern std::unique_ptr<Views::HUD> menuView;

	extern CommandManager commandManager;
	extern SelectionManager<std::shared_ptr<Objects::Object>> currentObjectSelection;
	extern TickManager tickManager;

	extern bool quit; // TODO: move this to GameManager

	namespace RuntimeObjects {
		extern std::shared_ptr<Objects::Object> player;
		extern std::shared_ptr<Objects::Object> arrow1;
		extern std::shared_ptr<Objects::Object> arrow2;
		extern std::shared_ptr<Objects::Object> hudArrow;
		extern std::shared_ptr<Objects::Object> camera;

		extern std::deque<std::shared_ptr<Objects::Bullet>> bullets;
	};

	namespace RuntimeShapes {
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

		extern std::shared_ptr<Shapes::Circle> hudCircle;

		extern std::shared_ptr<Shapes::Line> crosshairLine1;
		extern std::shared_ptr<Shapes::Line> crosshairLine2;
		extern std::shared_ptr<Shapes::HollowCircle> crosshairCircle1;
	};

	void init();
}