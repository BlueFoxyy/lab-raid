#include <init.h>

namespace Global {
	std::unique_ptr<FPSmanager> fpsManager;
	std::unique_ptr<Views::Camera> playerCamera;
	std::unique_ptr<Views::HUD> hudView;
	std::unique_ptr<Views::HUD> menuView;

	std::shared_ptr<Objects::Object> playerObject, arrowObject1;
	std::shared_ptr<Shapes::Circle> yellowCircle;
	std::shared_ptr<Shapes::Circle> greenCircle;
	std::shared_ptr<Shapes::Circle> blueCircle;
	std::shared_ptr<Shapes::Circle> redCircle;
	std::shared_ptr<Shapes::Circle> purpleCircle;

	std::shared_ptr<Shapes::HollowCircle> hollowCircle1;
	std::shared_ptr<Shapes::Line> line1;
	std::shared_ptr<Shapes::Line> line2;
	std::shared_ptr<Shapes::Line> line3;
	std::shared_ptr<Shapes::Line> line4;

	std::shared_ptr<Objects::Object> hudArrow;
	std::shared_ptr<Shapes::Circle> hudCircle;

	std::shared_ptr<Shapes::Line> crosshairLine1;
	std::shared_ptr<Shapes::Line> crosshairLine2;
	std::shared_ptr<Shapes::HollowCircle> crosshairCircle1;

	void init(void) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

		if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) < 0) {
			SDL_LogCritical(
				SDL_LOG_CATEGORY_APPLICATION,
				"Failed to initialize SDL. SDL_GetError(): %s",
				SDL_GetError()
			);
		}

		fpsManager = std::make_unique<FPSmanager>();
		SDL_initFramerate(fpsManager.get());
		SDL_setFramerate(fpsManager.get(), Config::framerate);

		playerCamera = std::make_unique<Views::Camera>();
		hudView = std::make_unique<Views::HUD>();

		playerObject = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "player" },
			playerCamera.get(),
			Vector2D{ 0, 0 },
			Vector2D{ 100, 100 }
		);
		arrowObject1 = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			playerCamera.get(),
			Vector2D{ -150, -150 },
			Vector2D{ 50, 37.5f }
		);
		yellowCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ -150, -150 },
			50,
			SDL_Color{ 127, 127, 0, 127 }
		);
		greenCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ -150, 150 },
			50,
			SDL_Color{ 0, 127, 0, 127 }
		);
		blueCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ 150, -150 },
			50,
			SDL_Color{ 0, 0, 127, 127 }
		);
		redCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ 150, 150 },
			50,
			SDL_Color{ 127, 0, 0, 127 }
		);
		purpleCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ 0, 0 },
			180,
			SDL_Color{ 220, 36, 200, 127 }
		);
		hollowCircle1 = std::make_shared<Shapes::HollowCircle>(
			Global::playerCamera.get(),
			Vector2D{ 0, 0 },
			100,
			10
		);
		line1 = std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ -100, -200 },
			Vector2D{ -100, 200 },
			10
		);
		line2 = std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ 100, -200 },
			Vector2D{ 100, 200 },
			10
		);
		line3 = std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ -200, -100 },
			Vector2D{ 200, -100 },
			10
		);
		line4 = std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ -200, 100 },
			Vector2D{ 200, 100 },
			10
		);

		hudArrow = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			hudView.get(),
			Vector2D{ 0, 12.5f },
			Vector2D{ 100, 75 }
		);
		hudCircle = std::make_shared<Shapes::Circle>(
			hudView.get(),
			hudView->getDimension() + Vector2D(-50, -50),
			50,
			SDL_Color{ 0xFF, 0x00, 0x00, 0x7F }
		);

		// CROSSHAIR
		crosshairLine1 = std::make_shared<Shapes::Line>(
			Global::hudView.get(),
			Vector2D{ 0, -31 },
			Vector2D{ 0, 31 },
			2
		);
		crosshairLine2 = std::make_shared<Shapes::Line>(
			Global::hudView.get(),
			Vector2D{ -31, 0 },
			Vector2D{ 31, 0 },
			2
		);
		crosshairCircle1 = std::make_shared<Shapes::HollowCircle>(
			Global::hudView.get(),
			Vector2D{ 0, 0 },
			13.5,
			2
		);

		// disable cursor, replaced with crosshair
		SDL_ShowCursor(SDL_DISABLE);

		// game
		Renderer::getInstance().registerObject(playerObject);
		Renderer::getInstance().registerObject(arrowObject1);
		Renderer::getInstance().registerObject(purpleCircle);
		Renderer::getInstance().registerObject(yellowCircle);
		Renderer::getInstance().registerObject(greenCircle);
		Renderer::getInstance().registerObject(blueCircle);
		Renderer::getInstance().registerObject(redCircle);
		Renderer::getInstance().registerObject(hollowCircle1);
		Renderer::getInstance().registerObject(line1);
		Renderer::getInstance().registerObject(line2);
		Renderer::getInstance().registerObject(line3);
		Renderer::getInstance().registerObject(line4);

		// hud
		Renderer::getInstance().registerObject(hudArrow);
		Renderer::getInstance().registerObject(hudCircle);
		Renderer::getInstance().registerObject(crosshairLine1);
		Renderer::getInstance().registerObject(crosshairLine2);
		Renderer::getInstance().registerObject(crosshairCircle1);
		
		// layer test
		Renderer::getInstance().moveLayerTop(playerObject);

		//object->rotate(M_PI / 2);
		//object->flipHorizontal();

		playerCamera->setPivotObject(playerObject);
		//playerCamera->setPivotObject(object);

		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized.");
	}
}