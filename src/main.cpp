#include <config.h>
#include <renderer.h>
#include <command/command.h>
#include <command_manager.h>
#include <shape/shapes.h>
#include <init.h>
#include <utility/selection_manager.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <vector>
#include <queue>

static const int TICKS_PER_SEC = 1000;
static const float gunOffset = 14;
static bool quit = false;
static float playerSpeed = 500;
static uint32_t curTick, prevTick, diffTick;

template<typename T>
static T timedDifference(T value) noexcept {
	return value * diffTick / TICKS_PER_SEC;
}

SelectionManager<std::shared_ptr<Objects::Object>> controlSelector;

class MoveUpCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		controlSelector.get()->move(
			Vector2D(0, -timedDifference(playerSpeed)).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveDownCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		controlSelector.get()->move(
			Vector2D(0, timedDifference(playerSpeed)).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveLeftCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		controlSelector.get()->move(
			Vector2D(-timedDifference(playerSpeed), 0).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveRightCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		controlSelector.get()->move(
			Vector2D(timedDifference(playerSpeed), 0).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class SwitchControlCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		if (InputHandler::getInstance().isKeyDown(SDLK_LSHIFT))
			controlSelector.prev();
		else
			controlSelector.next();
		Global::playerCamera->setPivotObject(controlSelector.get());
	}
};
class QuitCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		quit = true;
	}
};

/* CAMERA ZOOM TEST COMMANDS */
float zoomMultiplier = 1.0f;
float zoomSpeed = 1.5f;

class ZoomInCommand : public Commands::Command {
public:
	// for testing purposes
	static void command() noexcept {
		zoomMultiplier = std::min(zoomMultiplier + timedDifference(zoomSpeed), 4.0f);
		Global::playerCamera->setZoom(zoomMultiplier);
	}

	void execute(const ExecuteKey& key) noexcept override {
		command();
	}
};

class ZoomOutCommand : public Commands::Command {
public:
	// for testing purposes
	static void command() noexcept {
		zoomMultiplier = std::max(zoomMultiplier - timedDifference(zoomSpeed), 0.25f);
		Global::playerCamera->setZoom(zoomMultiplier);
	}

	void execute(const ExecuteKey& key) noexcept override {
		command();
	}
};

class ResetZoomCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		zoomMultiplier = 1.0f;
		Global::playerCamera->setZoom(zoomMultiplier);
	}
};
/* CAMERA ZOOM TEST COMMANDS */

/* CAMERA ROTATION TEST COMMANDS */
float rotateSpeed = 2.0f * (float)M_PI;
class RotateCameraClockwiseCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::playerCamera->rotate(timedDifference(rotateSpeed));
	}
};

class RotateCameraCounterClockwiseCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::playerCamera->rotate(-timedDifference(rotateSpeed));
	}
};
/* CAMERA ROTATION TEST COMMANDS */

/* LAYER TEST COMMANDS */
class PlayerLayerUpCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Renderer::getInstance().moveLayerUp(
			Global::playerObject
		);
	}
};
class PlayerLayerDownCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Renderer::getInstance().moveLayerDown(
			Global::playerObject
		);
	}
};
/* LAYER TEST COMMANDS */

/* BULLET TEST COMMANDS */
std::deque<std::shared_ptr<Objects::Bullet>> bullets;
class CreateBulletCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		auto bullet = std::make_shared<Objects::Bullet>(
			Global::playerCamera.get(),
			Global::playerObject->getPosition() + 
				(Vector2D{ 40 , gunOffset } + Objects::Bullet::bulletSize / 2).rotate(
				Global::playerObject->getAngle()
			),
			Global::playerObject->getAngle()
		);
		bullets.push_back(bullet);
		Renderer::getInstance().registerObject(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
	}
};
/* BULLET TEST COMMANDS */

/* FULLSCREEN TEST */
bool fullscreen = false;
class ToggleFullscreenCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		if (not fullscreen) {
			SDL_SetWindowFullscreen(Renderer::getInstance().getWindow(), SDL_WINDOW_FULLSCREEN);
		} else {
			SDL_SetWindowFullscreen(Renderer::getInstance().getWindow(), SDL_WINDOW_SHOWN);
		}
		fullscreen = not fullscreen;
	}
};
/* FULLSCREEN TEST */

// register commands
static void registerCommands(CommandManager& commandManager) {
	commandManager.registerCommand({
		{ {SDLK_q, KeyBind::Trigger::TAP} },
		{}
		}, std::make_shared<QuitCommand>());
	commandManager.registerCommand({
		{ {SDLK_w, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<MoveUpCommand>());
	commandManager.registerCommand({
		{ {SDLK_a, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<MoveLeftCommand>());
	commandManager.registerCommand({
		{ {SDLK_s, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<MoveDownCommand>());
	commandManager.registerCommand({
		{ {SDLK_d, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<MoveRightCommand>());
	commandManager.registerCommand({
		{ {SDLK_TAB, KeyBind::Trigger::TAP} },
		{}
		}, std::make_shared<SwitchControlCommand>());
	commandManager.registerCommand({
		{ {SDLK_RIGHTBRACKET, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<ZoomInCommand>());
	commandManager.registerCommand({
		{ {SDLK_LEFTBRACKET, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<ZoomOutCommand>());
	commandManager.registerCommand({
		{ {SDLK_BACKSLASH, KeyBind::Trigger::TAP} },
		{}
		}, std::make_shared<ResetZoomCommand>());
	commandManager.registerCommand({
		{ {SDLK_LEFT, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<RotateCameraCounterClockwiseCommand>());
	commandManager.registerCommand({
		{ {SDLK_RIGHT, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<RotateCameraClockwiseCommand>());
	commandManager.registerCommand({
		{ {SDLK_KP_8, KeyBind::Trigger::TAP} },
		{}
		}, std::make_shared<PlayerLayerUpCommand>());
	commandManager.registerCommand({
		{ {SDLK_KP_2, KeyBind::Trigger::TAP} },
		{}
		}, std::make_shared<PlayerLayerDownCommand>());
	commandManager.registerCommand({
		{},
		{ {MouseButton::RIGHT, KeyBind::Trigger::HOLD} }
		}, std::make_shared<CreateBulletCommand>());
	commandManager.registerCommand({
		{},
		{ {MouseButton::LEFT, KeyBind::Trigger::TAP} }
		}, std::make_shared<CreateBulletCommand>());
	commandManager.registerCommand({
		{ {SDLK_RETURN, KeyBind::Trigger::TAP}, {SDLK_LALT, KeyBind::Trigger::HOLD} },
		{}
		}, std::make_shared<ToggleFullscreenCommand>());
}

int main(int argc, char* argv[]) {
	// initialize
	Global::init();

	controlSelector.add(Global::playerObject);
	controlSelector.add(Global::arrowObject1);
	controlSelector.add(Global::arrowObject2);

	curTick = prevTick = 0;
	prevTick = SDL_GetTicks();

	SDL_setFramerate(Global::fpsManager.get(), 144);

	CommandManager commandManager;
	registerCommands(commandManager);

	int zoomInTimer = 0;
	int zoomOutTimer = 0;

	SDL_Event event;
	while (not quit) {
		// calculate elapsed ticks
		prevTick = curTick;
		curTick = SDL_GetTicks();
		diffTick = curTick - prevTick;

		// register inputs
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				InputHandler::getInstance().receiveEvent(event.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				InputHandler::getInstance().receiveEvent(event.button);
				break;
			case SDL_MOUSEWHEEL:
				InputHandler::getInstance().receiveEvent(event.wheel);
			}
		}

		// execute registered commands
		commandManager.update();

//		Global::playerObject->rotate(0.01);
//		Global::object->rotate(-0.02);
//		Global::playerObject->lookAt(Global::object->getPosition());

		// get cursor's virtual position in game coordinate system
		Vector2D cursorPosition =
			Global::playerCamera->transformFromRender(
				InputHandler::getInstance().getMousePosition()
			);

		// set player to look at cursor
		Global::playerObject->lookAt(cursorPosition);
		
		// smooth scrolling
		float scrollY = InputHandler::getInstance().pollMouseScroll().getY();
		if (scrollY > 0) {
			zoomInTimer = 20;
			zoomOutTimer = 0;
		}
		else if (scrollY < 0) {
			zoomInTimer = 0;
			zoomOutTimer = 20;
		}
		if (zoomInTimer > 0) {
			zoomInTimer--;
			ZoomInCommand::command();
		} else if (zoomOutTimer > 0) {
			zoomOutTimer--;
			ZoomOutCommand::command();
		}

		// adjust angle so that the gun points at the cursor instead of player look at the cursor
		Vector2D distVector = cursorPosition - Global::playerObject->getPosition();
		float offsetAngle = gunOffset / distVector.len();
//		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "offsetAngle = %lf", offsetAngle);
		Global::playerObject->rotate(offsetAngle);

		// set arrow to look at the player object.
		Global::arrowObject1->lookAt(Global::playerObject->getPosition());
		Global::arrowObject2->lookAt(Global::playerObject->getPosition());

		// Set crosshair position
		Vector2D hudCursorPosition = Global::hudView->transformFromRender(
			InputHandler::getInstance().getMousePosition()
		);
		Global::crosshairLine1->setBeginPoint(
			hudCursorPosition + Vector2D{ 0, -31 }
		);
		Global::crosshairLine1->setEndPoint(
			hudCursorPosition + Vector2D{ 0, 31 }
		);
		Global::crosshairLine2->setBeginPoint(
			hudCursorPosition + Vector2D{ -31, 0 }
		);
		Global::crosshairLine2->setEndPoint(
			hudCursorPosition + Vector2D{ 31, 0 }
		);
		Global::crosshairCircle1->setCenter(
			hudCursorPosition
		);

		/*
		Shapes::Line line1({0, 0}, {1600, 900}, 3, {0xFF, 0x00, 0x00, 0x7F});
		Shapes::Line line2({ 0, 900 }, { 1600, 0 }, 2, {0x00, 0xFF, 0x00, 0x7F});

		line1.draw(Renderer::getInstance().getRawRenderer(), Global::hudView.get());
		line2.draw(Renderer::getInstance().getRawRenderer(), Global::hudView.get());

		Shapes::Circle circle({ 0, 0 }, 250, { 0, 0, 0xFF, 0x7F });

		circle.draw(Renderer::getInstance().getRawRenderer(), Global::playerCamera.get());

		SDL_SetRenderDrawColor(Renderer::getInstance().getRawRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		*/

		for (auto bullet : bullets) {
			bullet->update();
		}
		while (not bullets.empty()) {
			const auto& bullet = bullets.front();
			if (bullet->getAliveTime() > 2000)
				bullets.pop_front();
			else
				break;
		}

		bool bulletHitArrow1 = false;
		bool bulletHitArrow2 = false;
		bool bulletHitHudArrow = false;
		for (auto bullet : bullets) {
			bulletHitArrow1 = bulletHitArrow1 or bullet->collideWith(*Global::arrowObject1);
			bulletHitArrow2 = bulletHitArrow2 or bullet->collideWith(*Global::arrowObject2);
			bulletHitHudArrow = bulletHitHudArrow or bullet->collideWith(*Global::hudArrow);
		}
		if (bulletHitArrow1) {
			Global::hudCircle->setColor({ 0xFF, 0x00, 0x00, 0xFF });
		} else if (bulletHitArrow2) {
			Global::hudCircle->setColor({ 0x00, 0xFF, 0x00, 0xFF });
		} else if (bulletHitHudArrow) {
			Global::hudCircle->setColor({ 0x00, 0x00, 0xFF, 0xFF });
		} else {
			Global::hudCircle->setColor({ 0xFF, 0xFF, 0xFF, 0x7F });
		}

		// rotate hud arrow
		Global::hudArrow->rotate(timedDifference(2*M_PI));

		try {
			Renderer::getInstance().render({});
		} catch (std::logic_error& e) {
			SDL_LogCritical(
				SDL_LOG_CATEGORY_APPLICATION, 
				"Failed to render. what(): %s", 
				e.what()
			);
			exit(EXIT_FAILURE);
		}

		SDL_framerateDelay(Global::fpsManager.get());

		SDL_LogInfo(
			SDL_LOG_CATEGORY_APPLICATION,
			"FPS: %d",
			SDL_getFramerate(Global::fpsManager.get())
		);		
	}

	SDL_Quit();
	return 0;
}