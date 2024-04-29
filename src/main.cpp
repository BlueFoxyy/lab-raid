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
T timedDifference(T value) noexcept {
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
	void execute(ExecuteKey key) noexcept override {
		zoomMultiplier = std::min(zoomMultiplier + timedDifference(zoomSpeed), 4.0f);
		Global::playerCamera->setZoom(zoomMultiplier);
	}
};

class ZoomOutCommand : public Commands::Command {
public:
	void execute(ExecuteKey key) noexcept override {
		zoomMultiplier = std::max(zoomMultiplier - timedDifference(zoomSpeed), 0.25f);
		Global::playerCamera->setZoom(zoomMultiplier);
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
float rotateSpeed = 2 * M_PI;
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
	commandManager.registerCommand(KeyBind(
		{ {SDLK_q, KeyBind::Trigger::TAP} }, {}
	), std::make_shared<QuitCommand>());
	commandManager.registerCommand({
		{ {SDLK_w, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<MoveUpCommand>());
	commandManager.registerCommand({
		{ {SDLK_a, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<MoveLeftCommand>());
	commandManager.registerCommand({
		{ {SDLK_s, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<MoveDownCommand>());
	commandManager.registerCommand({
		{ {SDLK_d, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<MoveRightCommand>());
	commandManager.registerCommand({
		{ {SDLK_TAB, KeyBind::Trigger::TAP}, {SDLK_LSHIFT, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<SwitchControlCommand>());
	commandManager.registerCommand({
		{ {SDLK_RIGHTBRACKET, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<ZoomInCommand>());
	commandManager.registerCommand({
		{ {SDLK_LEFTBRACKET, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<ZoomOutCommand>());
	commandManager.registerCommand({
		{ {SDLK_BACKSLASH, KeyBind::Trigger::TAP} }, {}
		}, std::make_shared<ResetZoomCommand>());
	commandManager.registerCommand({
		{ {SDLK_LEFT, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<RotateCameraCounterClockwiseCommand>());
	commandManager.registerCommand({
		{ {SDLK_RIGHT, KeyBind::Trigger::HOLD} }, {}
		}, std::make_shared<RotateCameraClockwiseCommand>());
	std::shared_ptr<Shapes::HollowCircle> hollowCircle1
		= std::make_shared<Shapes::HollowCircle>(
			Global::playerCamera.get(),
			10,
			Vector2D{ 0, 0 },
			100
		);
	std::shared_ptr<Shapes::Line> line1
		= std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ -100, -200 },
			Vector2D{ -100, 200 },
			10
		);
	std::shared_ptr<Shapes::Line> line2
		= std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ 100, -200 },
			Vector2D{ 100, 200 },
			10
		);
	std::shared_ptr<Shapes::Line> line3
		= std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ -200, -100 },
			Vector2D{ 200, -100 },
			10
		);
	std::shared_ptr<Shapes::Line> line4
		= std::make_shared<Shapes::Line>(
			Global::playerCamera.get(),
			Vector2D{ -200, 100 },
			Vector2D{ 200, 100 },
			10
		);
	Renderer::getInstance().registerObject(hollowCircle1);
	Renderer::getInstance().registerObject(line1);
	Renderer::getInstance().registerObject(line2);
	Renderer::getInstance().registerObject(line3);
	Renderer::getInstance().registerObject(line4);

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

		Vector2D distVector = Global::arrowObject1->getPosition() - Global::playerObject->getPosition();
		float offsetAngle = distVector.len() >= 1 ? asin(
			gunOffset / distVector.len()
		) : 0;
//		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "offsetAngle = %lf", offsetAngle);
		Global::playerObject->rotate(offsetAngle);
		Global::arrowObject1->lookAt(Global::playerObject->getPosition());

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