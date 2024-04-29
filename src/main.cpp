#include <config.h>
#include <renderer.h>
#include <command/command.h>
#include <command_manager.h>
#include <init.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <stdexcept>

#include <shape/shapes.h>

static const int TICKS_PER_SEC = 1000;
static const float gunOffset = 16.5;
static bool quit = false;
static float playerSpeed = 500;
static std::weak_ptr<Objects::Object> controlledObject;
static uint32_t curTick, prevTick, diffTick;

class MoveUpCommand : public Commands::Command {
public:
	void execute(void) noexcept override {
		controlledObject.lock()->move({ 0, -playerSpeed * diffTick / TICKS_PER_SEC });
	}
};
class MoveDownCommand : public Commands::Command {
public:
	void execute(void) noexcept override {
		controlledObject.lock()->move({ 0, playerSpeed * diffTick / TICKS_PER_SEC });
	}
};
class MoveLeftCommand : public Commands::Command {
public:
	void execute(void) noexcept override {
		controlledObject.lock()->move({ -playerSpeed * diffTick / TICKS_PER_SEC, 0 });
	}
};
class MoveRightCommand : public Commands::Command {
public:
	void execute(void) noexcept override {
		controlledObject.lock()->move({ playerSpeed * diffTick / TICKS_PER_SEC, 0 });
	}
};
class QuitCommand : public Commands::Command {
public:
	void execute(void) noexcept override {
		quit = true;
	}
};
class SwitchControlCommand : public Commands::Command {
public:
	void execute(void) noexcept override {
		auto tempObject = controlledObject.lock();
		if (tempObject == Global::playerObject)
			controlledObject = Global::arrowObject1;
		else
			controlledObject = Global::playerObject;
		Global::playerCamera->setPivotObject(controlledObject.lock());
	}
};
  
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

		Vector2D distVector = Global::arrowObject1->getPosition() - Global::playerObject->getPosition();
		float offsetAngle = distVector.len() >= 1 ? asin(
			gunOffset / distVector.len()
		) : 0;
//		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "offsetAngle = %lf", offsetAngle);
		Global::playerObject->rotate(
			offsetAngle
		);
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