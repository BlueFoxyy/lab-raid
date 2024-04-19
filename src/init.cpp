#include <object/object.h>
#include <view/views.h>
#include <renderer.h>
#include <SDL.h>
#include <SDL2_framerate.h>
#include <memory>
#include <string>
#include <format>
#include <vector>
#include <geometry/vector2d.h>

namespace Global {
	extern std::unique_ptr<FPSmanager> fpsManager;
	extern std::unique_ptr<Views::Camera> playerCamera;
	extern std::unique_ptr<Views::HUD> hud;

	extern std::shared_ptr<Objects::Object> playerObject, object;

	void init(void) {
		if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) < 0) {
			throw std::logic_error(std::format(
				"Failed to initialize SDL. SDL_GetError(): {}",
				SDL_GetError()
			));
		}

		SDL_initFramerate(fpsManager.get());

		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);

		playerCamera = std::make_unique<Views::Camera>();

		playerObject = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "player" },
			playerCamera.get(),
			Vector2D{ 0, 0 },
			Vector2D{ 128, 128 }
		);
		if (not Renderer::getInstance().registerObject(playerObject)) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to register object.");
			exit(EXIT_FAILURE);
		}

		object = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			playerCamera.get(),
			Vector2D{ -100, -100 },
			Vector2D{ 128, 128 }
		);
		if (not Renderer::getInstance().registerObject(object)) {
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