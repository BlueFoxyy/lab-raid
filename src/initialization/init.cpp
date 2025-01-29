#include <init.h>

namespace Global {

	void init(void) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

		if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) < 0) {
			SDL_LogCritical(
				SDL_LOG_CATEGORY_APPLICATION,
				"Failed to initialize SDL. SDL_GetError(): %s",
				SDL_GetError()
			);
			exit(EXIT_FAILURE);
		}
		fpsManager = std::make_unique<FPSmanager>();
		SDL_initFramerate(fpsManager.get());
		SDL_setFramerate(fpsManager.get(), Config::framerate);

		// views
		playerCamera = std::make_unique<Views::Camera>();
		hudView = std::make_unique<Views::HUD>();

		// disable cursor, replaced with crosshair
		SDL_ShowCursor(SDL_DISABLE);

		initObjects();
		initShapes();
		initCommands();

		// render layers
		Renderer::getInstance().registerObject(RuntimeObjects::player);
		Renderer::getInstance().registerObject(RuntimeObjects::arrow1);
		Renderer::getInstance().registerObject(RuntimeObjects::arrow2);
		Renderer::getInstance().registerObject(RuntimeShapes::purpleCircle);
		Renderer::getInstance().registerObject(RuntimeShapes::yellowCircle);
		Renderer::getInstance().registerObject(RuntimeShapes::greenCircle);
		Renderer::getInstance().registerObject(RuntimeShapes::blueCircle);
		Renderer::getInstance().registerObject(RuntimeShapes::redCircle);
		Renderer::getInstance().registerObject(RuntimeShapes::hollowCircle1);
		Renderer::getInstance().registerObject(RuntimeShapes::line1);
		Renderer::getInstance().registerObject(RuntimeShapes::line2);
		Renderer::getInstance().registerObject(RuntimeShapes::line3);
		Renderer::getInstance().registerObject(RuntimeShapes::line4);

		// hud
		Renderer::getInstance().registerObject(RuntimeShapes::hudCircle);
		Renderer::getInstance().registerObject(RuntimeShapes::crosshairLine1);
		Renderer::getInstance().registerObject(RuntimeShapes::crosshairLine2);
		Renderer::getInstance().registerObject(RuntimeShapes::crosshairCircle1);
		
		// layer test
		Renderer::getInstance().moveLayerTop(RuntimeObjects::player);

		//object->rotate(M_PI / 2);
		//object->flipHorizontal();

		playerCamera->setPivotObject(RuntimeObjects::camera);
		//playerCamera->setPivotObject(playerObject);
		//playerCamera->setPivotObject(object);

		quit = false;

		tickManager.init();

		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized.");
	}
}