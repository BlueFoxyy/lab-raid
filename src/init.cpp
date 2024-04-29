#include <init.h>

namespace Global {
	std::unique_ptr<FPSmanager> fpsManager;
	std::unique_ptr<Views::Camera> playerCamera;
	std::unique_ptr<Views::HUD> hudView;
	std::unique_ptr<Views::HUD> menuView;

	std::shared_ptr<Objects::Object> playerObject, arrowObject1;
	std::shared_ptr<Objects::Object> arrowObject2;
	std::shared_ptr<Shapes::Circle> yellowCircle;
	std::shared_ptr<Shapes::Circle> greenCircle;
	std::shared_ptr<Shapes::Circle> blueCircle;
	std::shared_ptr<Shapes::Circle> redCircle;
	std::shared_ptr<Shapes::Circle> purpleCircle;

	void init(void) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
		//SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

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
		if (not Renderer::getInstance().registerObject(playerObject)) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to register playerObject.");
			exit(EXIT_FAILURE);
		}

		arrowObject1 = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			playerCamera.get(),
			Vector2D{ -150, -150 },
			Vector2D{ 50, 50 }
		);

		if (not Renderer::getInstance().registerObject(arrowObject1)) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to register arrowObject1.");
			exit(EXIT_FAILURE);
		}
		arrowObject2 = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			hudView.get(),
			Vector2D{ 0, 0 },
			Vector2D{ 100, 100 }
		);
		if (not Renderer::getInstance().registerObject(arrowObject2)) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to register arrowObject2.");
			exit(EXIT_FAILURE);
		}

		yellowCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ -150, -150 },
			50,
			SDL_Color{ 127, 127, 0, 127 }
		);
		Renderer::getInstance().registerObject(yellowCircle);
		greenCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ -150, 150 },
			50,
			SDL_Color{ 0, 127, 0, 127 }
		);
		Renderer::getInstance().registerObject(greenCircle);
		blueCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ 150, -150 },
			50,
			SDL_Color{ 0, 0, 127, 127 }
		);
		Renderer::getInstance().registerObject(blueCircle);
		redCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ 150, 150 },
			50,
			SDL_Color{ 127, 0, 0, 127 }
		);
		Renderer::getInstance().registerObject(redCircle);
		purpleCircle = std::make_shared<Shapes::Circle>(
			playerCamera.get(),
			Vector2D{ 0, 0 },
			180,
			SDL_Color{ 220, 36, 200, 127 }
		);
		Renderer::getInstance().registerObject(purpleCircle);
		
		//object->rotate(M_PI / 2);
		//object->flipHorizontal();

		playerCamera->setPivotObject(playerObject);
		//playerCamera->setPivotObject(object);

		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized.");
	}
}