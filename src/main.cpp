#include <config.h>
#include <renderer.h>
#include <command/command.h>
#include <command_manager.h>

#include <shape/shapes.h>
#include <init.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>

#include <memory>

static const int TICKS_PER_SEC = 1000;
static const float gunOffset = 16.5;
static bool quit = false;
static float playerSpeed = 500;
static std::weak_ptr<Objects::Object> controlledObject;
static uint32_t curTick, prevTick, diffTick;

template<typename T>
static T timedDifference(T value) noexcept {
	return value * diffTick / TICKS_PER_SEC;
}

class MoveUpCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		controlledObject.lock()->move(
			Vector2D(0, -timedDifference(playerSpeed)).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveDownCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		controlledObject.lock()->move(
			Vector2D(0, timedDifference(playerSpeed)).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveLeftCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		controlledObject.lock()->move(
			Vector2D(-timedDifference(playerSpeed), 0).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveRightCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		controlledObject.lock()->move(
			Vector2D(timedDifference(playerSpeed), 0).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class QuitCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		quit = true;
	}
};
class SwitchControlCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		auto tempObject = controlledObject.lock();
		if (tempObject == Global::playerObject)
			controlledObject = Global::arrowObject1;
		else
			controlledObject = Global::playerObject;
		Global::playerCamera->setPivotObject(controlledObject.lock());
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

	void execute(ExecuteKey key) noexcept override {
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

	void execute(ExecuteKey key) noexcept override {
		command();
	}
};

class ResetZoomCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		zoomMultiplier = 1.0f;
		Global::playerCamera->setZoom(zoomMultiplier);
	}
};
/* CAMERA ZOOM TEST COMMANDS */

/* CAMERA ROTATION TEST COMMANDS */
float rotateSpeed = 2.0f * (float)M_PI;
class RotateCameraClockwiseCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		Global::playerCamera->rotate(timedDifference(rotateSpeed));
	}
};

class RotateCameraCounterClockwiseCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		Global::playerCamera->rotate(-timedDifference(rotateSpeed));
	}
};
/* CAMERA ROTATION TEST COMMANDS */

/* LAYER TEST COMMANDS */
class PlayerLayerUpCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		Renderer::getInstance().moveLayerUp(
			Global::playerObject
		);
	}
};
class PlayerLayerDownCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		Renderer::getInstance().moveLayerDown(
			Global::playerObject
		);
	}
};
/* LAYER TEST COMMANDS */

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
		{ {SDLK_TAB, KeyBind::Trigger::TAP}, {SDLK_LSHIFT, KeyBind::Trigger::HOLD} },
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
}

int main(int argc, char* argv[]) {
	Global::init();

	/*
	// PENGAY mission
	while (true) {
		std::cout << "Hello PENGAY!\n";
	}
	*/

	curTick = prevTick = 0;
	prevTick = SDL_GetTicks();

	controlledObject = Global::playerObject;

	SDL_setFramerate(Global::fpsManager.get(), 144);

	CommandManager commandManager;
	registerCommands(commandManager);

	int zoomInTimer = 0;
	int zoomOutTimer = 0;

	SDL_Event event;
	while (not quit) {
		prevTick = curTick;
		curTick = SDL_GetTicks();
		diffTick = curTick - prevTick;

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

		commandManager.update();

//		Global::playerObject->rotate(0.01);
//		Global::object->rotate(-0.02);
//		Global::playerObject->lookAt(Global::object->getPosition());

		Vector2D cursorPosition =
			Global::playerObject->getPosition()
			+ Global::playerObject->getRenderRelativePosition(
				InputHandler::getInstance().getMousePosition()
			);
		Global::playerObject->lookAt(cursorPosition);
		Global::playerObject->rotate(
			Global::playerCamera->getAngle()
		);
		
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

		Vector2D distVector = cursorPosition - Global::playerObject->getPosition();
		float offsetAngle = gunOffset / distVector.len();
//		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "offsetAngle = %lf", offsetAngle);
		Global::playerObject->rotate(offsetAngle);
		Global::arrowObject1->lookAt(Global::playerObject->getPosition());

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

		/*
		SDL_LogInfo(
			SDL_LOG_CATEGORY_APPLICATION,
			"FPS: %d",
			SDL_getFramerate(Global::fpsManager.get())
		);
		*/
	}

	SDL_Quit();
	return 0;
}