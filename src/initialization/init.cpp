#include <SDL2/SDL.h>
#include <init.h>
#include "../commands.hpp"

namespace Global {

	std::unique_ptr<FPSmanager> fpsManager;
	std::unique_ptr<Views::Camera> playerCamera;
	std::unique_ptr<Views::HUD> hudView;
	std::unique_ptr<Views::HUD> menuView;

	CommandManager commandManager;
	SelectionManager<std::shared_ptr<Objects::Object>> currentObjectSelection;
	TickManager tickManager;

	bool quit; // TODO: move this to GameManager

	namespace RuntimeObjects {
		std::shared_ptr<Objects::Object> player;
		std::shared_ptr<Objects::Object> arrow1;
		std::shared_ptr<Objects::Object> arrow2;
		std::shared_ptr<Objects::Object> hudArrow;
		std::shared_ptr<Objects::Object> camera;

		std::deque<std::shared_ptr<Objects::Bullet>> bullets;
	};

	namespace RuntimeShapes {
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

		std::shared_ptr<Shapes::Circle> hudCircle;

		std::shared_ptr<Shapes::Line> crosshairLine1;
		std::shared_ptr<Shapes::Line> crosshairLine2;
		std::shared_ptr<Shapes::HollowCircle> crosshairCircle1;
	};

	void initObjects(void);
	void initShapes(void);
	void initCommands(void);

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
		Renderer::getInstance().registerObject(RuntimeObjects::hudArrow);
		Renderer::getInstance().registerObject(RuntimeShapes::hudCircle);
		Renderer::getInstance().registerObject(RuntimeShapes::crosshairLine1);
		Renderer::getInstance().registerObject(RuntimeShapes::crosshairLine2);
		Renderer::getInstance().registerObject(RuntimeShapes::crosshairCircle1);
		
		// layer test
		Renderer::getInstance().moveLayerTop(RuntimeObjects::player);

		//object->rotate(M_PI / 2);
		//object->flipHorizontal();

		playerCamera->setPivotObject(RuntimeObjects::camera);

		quit = false;

		tickManager.init();

		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized.");
	}

	void initObjects(void) {
		using namespace RuntimeObjects;
		player = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "player" },
			playerCamera.get(),
			Vector2D{ 0, 0 },
			Vector2D{ 100, 100 }
		);
		arrow1 = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			playerCamera.get(),
			Vector2D{ -150, -150 },
			Vector2D{ 50, 37.5f }
		);
		arrow2 = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			playerCamera.get(),
			Vector2D{ 150, 150 },
			Vector2D{ 50, 37.5f }
		);
		hudArrow = std::make_shared<Objects::Object>(
			std::vector<std::string>{ "arrow" },
			hudView.get(),
			Vector2D{ 0, 12.5f },
			Vector2D{ 100, 75 }
		);
		camera = std::make_shared<Objects::Object>(
			std::vector<std::string>{}, // no texture
			playerCamera.get(),
			Vector2D{ 0, 0 },
			Vector2D{ 0, 0 }
		);

		currentObjectSelection.add(player);
		currentObjectSelection.add(arrow1);
		currentObjectSelection.add(arrow2);
	}

	void initShapes(void) {
		using namespace RuntimeShapes;

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
			playerCamera.get(),
			Vector2D{ 0, 0 },
			100,
			10
		);
		line1 = std::make_shared<Shapes::Line>(
			playerCamera.get(),
			Vector2D{ -100, -200 },
			Vector2D{ -100, 200 },
			10
		);
		line2 = std::make_shared<Shapes::Line>(
			playerCamera.get(),
			Vector2D{ 100, -200 },
			Vector2D{ 100, 200 },
			10
		);
		line3 = std::make_shared<Shapes::Line>(
			playerCamera.get(),
			Vector2D{ -200, -100 },
			Vector2D{ 200, -100 },
			10
		);
		line4 = std::make_shared<Shapes::Line>(
			playerCamera.get(),
			Vector2D{ -200, 100 },
			Vector2D{ 200, 100 },
			10
		);
		hudCircle = std::make_shared<Shapes::Circle>(
			hudView.get(),
			hudView->getDimension() + Vector2D(-50, -50),
			50,
			SDL_Color{ 0xFF, 0xFF, 0xFF, 0x7F }
		);
		// CROSSHAIR
		crosshairLine1 = std::make_shared<Shapes::Line>(
			hudView.get(),
			Vector2D{ 0, -31 },
			Vector2D{ 0, 31 },
			2
		);
		crosshairLine2 = std::make_shared<Shapes::Line>(
			hudView.get(),
			Vector2D{ -31, 0 },
			Vector2D{ 31, 0 },
			2
		);
		crosshairCircle1 = std::make_shared<Shapes::HollowCircle>(
			hudView.get(),
			Vector2D{ 0, 0 },
			13.5,
			2
		);
	}

	// register commands
	void initCommands() {
		Global::commandManager.registerCommand({
			{ {SDLK_q, KeyBind::Trigger::TAP} },
			{}
			}, std::make_shared<QuitCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_w, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<MoveUpCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_a, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<MoveLeftCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_s, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<MoveDownCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_d, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<MoveRightCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_TAB, KeyBind::Trigger::TAP} },
			{}
			}, std::make_shared<SwitchControlCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_RIGHTBRACKET, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<ZoomInCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_LEFTBRACKET, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<ZoomOutCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_BACKSLASH, KeyBind::Trigger::TAP} },
			{}
			}, std::make_shared<ResetZoomCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_LEFT, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<RotateCameraCounterClockwiseCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_RIGHT, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<RotateCameraClockwiseCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_KP_8, KeyBind::Trigger::TAP} },
			{}
			}, std::make_shared<PlayerLayerUpCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_KP_2, KeyBind::Trigger::TAP} },
			{}
			}, std::make_shared<PlayerLayerDownCommand>());
		Global::commandManager.registerCommand({
			{},
			{ {MouseButton::RIGHT, KeyBind::Trigger::HOLD} }
			}, std::make_shared<CreateBulletCommand>());
		Global::commandManager.registerCommand({
			{},
			{ {MouseButton::LEFT, KeyBind::Trigger::TAP} }
			}, std::make_shared<CreateBulletCommand>());
		Global::commandManager.registerCommand({
			{ {SDLK_RETURN, KeyBind::Trigger::TAP}, {SDLK_LALT, KeyBind::Trigger::HOLD} },
			{}
			}, std::make_shared<ToggleFullscreenCommand>());
	}
}