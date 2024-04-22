#include <init.h>

namespace Global {
	extern std::unique_ptr<FPSmanager> fpsManager;
	extern std::unique_ptr<Views::Camera> playerCamera;
	extern std::unique_ptr<Views::HUD> hud;

	extern std::shared_ptr<Objects::Object> playerObject, arrowObject;

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

		playerObject = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "player" },
			playerCamera.get(),
			Vector2D{ 0, 0 },
			Vector2D{ 128, 128 }
		);
		if (not Renderer::getInstance().registerObject(playerObject)) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to register arrowObject.");
			exit(EXIT_FAILURE);
		}

		arrowObject = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			playerCamera.get(),
			Vector2D{ -100, -100 },
			Vector2D{ 128, 128 }
		);
		if (not Renderer::getInstance().registerObject(arrowObject)) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to register object.");
			exit(EXIT_FAILURE);
		}

		//object->rotate(M_PI / 2);
		//object->flipHorizontal();

		playerCamera->setPivotObject(playerObject);
		//playerCamera->setPivotObject(object);

		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized.");
	}
}