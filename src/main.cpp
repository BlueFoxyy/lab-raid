#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <input_handler.h>
#include <renderer.h>
#include <exception>

#include "commands.hpp" // TODO: remove this line, included for testing purposes (zoom in / out command definition)

int main(int argc, char* argv[]) {
	// initialize
	Global::init();

	//SDL_setFramerate(Global::fpsManager.get(), 144);

	int zoomInTimer = 0;
	int zoomOutTimer = 0;

	SDL_Event event;
	while (not Global::quit) {
		// calculate elapsed ticks
		Global::tickManager.update();

		// register inputs
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				Global::quit = true;
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
		Global::commandManager.update();

//		Global::playerObject->rotate(0.01);
//		Global::object->rotate(-0.02);
//		Global::playerObject->lookAt(Global::object->getPosition());

		// get cursor's virtual position in game coordinate system
		Vector2D playerCursorPosition =
			Global::playerCamera->transformFromRender(
				InputHandler::getInstance().getMousePosition()
			);

		// set player to look at cursor
		Global::RuntimeObjects::player->lookAt(playerCursorPosition);
		
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
		Vector2D distVector = playerCursorPosition - Global::RuntimeObjects::player->getPosition();
		float offsetAngle = Config::gunOffset / distVector.len();
//		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "offsetAngle = %lf", offsetAngle);
		Global::RuntimeObjects::player->rotate(offsetAngle);

		// set arrow to look at the player object.
		Global::RuntimeObjects::arrow1->lookAt(Global::RuntimeObjects::player->getPosition());
		Global::RuntimeObjects::arrow2->lookAt(Global::RuntimeObjects::player->getPosition());

		// Set crosshair position
		Vector2D cursorPosition = InputHandler::getInstance().getMousePosition();
		Vector2D hudCursorPosition = Global::hudView->transformFromRender(
			cursorPosition
		);
		Global::RuntimeShapes::crosshairLine1->setBeginPoint(
			hudCursorPosition + Vector2D{ 0, -31 }
		);
		Global::RuntimeShapes::crosshairLine1->setEndPoint(
			hudCursorPosition + Vector2D{ 0, 31 }
		);
		Global::RuntimeShapes::crosshairLine2->setBeginPoint(
			hudCursorPosition + Vector2D{ -31, 0 }
		);
		Global::RuntimeShapes::crosshairLine2->setEndPoint(
			hudCursorPosition + Vector2D{ 31, 0 }
		);
		Global::RuntimeShapes::crosshairCircle1->setCenter(
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

		auto& bullets = Global::RuntimeObjects::bullets;
		for (auto& bullet : bullets) {
			bullet->update();
		}
		while (not bullets.empty()) {
			const auto& bullet =bullets.front();
			if (bullet->getAliveTime() > 2000)
				bullets.pop_front();
			else
				break;
		}

		bool bulletHitArrow1 = false;
		bool bulletHitArrow2 = false;
		bool bulletHitHudArrow = false;
		for (auto& bullet : Global::RuntimeObjects::bullets) {
			bulletHitArrow1 = bulletHitArrow1 or bullet->collideWith(*Global::RuntimeObjects::arrow1);
			bulletHitArrow2 = bulletHitArrow2 or bullet->collideWith(*Global::RuntimeObjects::arrow2);
			bulletHitHudArrow = bulletHitHudArrow or bullet->collideWith(*Global::RuntimeObjects::hudArrow);
		}
		if (bulletHitArrow1) {
			Global::RuntimeShapes::hudCircle->setColor({ 0xFF, 0x00, 0x00, 0xFF });
		} else if (bulletHitArrow2) {
			Global::RuntimeShapes::hudCircle->setColor({ 0x00, 0xFF, 0x00, 0xFF });
		} else if (bulletHitHudArrow) {
			Global::RuntimeShapes::hudCircle->setColor({ 0x00, 0x00, 0xFF, 0xFF });
		} else {
			Global::RuntimeShapes::hudCircle->setColor({ 0xFF, 0xFF, 0xFF, 0x7F });
		}

		// rotate hud arrow
		Global::RuntimeObjects::hudArrow->rotate(2 * M_PI * Global::tickManager.getDiffTick() / TICKS_PER_SEC);

		// set camera object position
		Global::RuntimeObjects::camera->setPosition(
			0.75 * Global::RuntimeObjects::player->getPosition()
			+ 0.25 * playerCursorPosition
		);

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

		/*SDL_LogInfo(
			SDL_LOG_CATEGORY_APPLICATION,
			"FPS: %d",
			SDL_getFramerate(Global::fpsManager.get())
		);*/		
	}

	SDL_Quit();
	return 0;
}