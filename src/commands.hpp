#pragma once

#include <init.h>
#include <managers/command_manager.h>

static float playerSpeed = 500;
class MoveUpCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(0, -playerSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveDownCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(0, playerSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveLeftCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(-playerSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC, 0).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveRightCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(playerSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC, 0).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class SwitchControlCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		if (InputHandler::getInstance().isKeyDown(SDLK_LSHIFT))
			Global::currentObjectSelection.prev();
		else
			Global::currentObjectSelection.next();
		if (Global::currentObjectSelection.get() == Global::RuntimeObjects::player)
			Global::playerCamera->setPivotObject(Global::RuntimeObjects::player);
		else
			Global::playerCamera->setPivotObject(Global::currentObjectSelection.get());
	}
};
class QuitCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::quit = true;
	}
};

/* CAMERA ZOOM TEST COMMANDS */
static float zoomMultiplier = 1.0f;
static float zoomSpeed = 1.5f;

class ZoomInCommand : public Commands::Command {
public:
	// for testing purposes
	static void command() noexcept {
		zoomMultiplier = std::min(zoomMultiplier + zoomSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC, 4.0f);
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
		zoomMultiplier = std::max(zoomMultiplier - zoomSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC, 0.25f);
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
static float rotateSpeed = 2.0f * (float)M_PI;
class RotateCameraClockwiseCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::playerCamera->rotate(rotateSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC);
	}
};

class RotateCameraCounterClockwiseCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::playerCamera->rotate(-rotateSpeed * Global::tickManager.getDiffTick() / TICKS_PER_SEC);
	}
};
/* CAMERA ROTATION TEST COMMANDS */

/* LAYER TEST COMMANDS */
class PlayerLayerUpCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Renderer::getInstance().moveLayerUp(
			Global::RuntimeObjects::player
		);
	}
};
class PlayerLayerDownCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Renderer::getInstance().moveLayerDown(
			Global::RuntimeObjects::player
		);
	}
};
/* LAYER TEST COMMANDS */

/* BULLET TEST COMMANDS */
class CreateBulletCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		auto bullet = std::make_shared<Objects::Bullet>(
			Global::playerCamera.get(),
			Global::RuntimeObjects::player->getPosition() +
			Vector2D{ 40 , Config::gunOffset }.rotate(
				Global::RuntimeObjects::player->getAngle()
			),
			Global::RuntimeObjects::player->getAngle()
		);
		Global::RuntimeObjects::bullets.push_back(bullet);
		Renderer::getInstance().registerObject(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
		Renderer::getInstance().moveLayerDown(bullet);
	}
};
/* BULLET TEST COMMANDS */

/* FULLSCREEN TEST */
static bool fullscreen = false;
class ToggleFullscreenCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		if (not fullscreen) {
			SDL_SetWindowFullscreen(Renderer::getInstance().getWindow(), SDL_WINDOW_FULLSCREEN);
		}
		else {
			SDL_SetWindowFullscreen(Renderer::getInstance().getWindow(), SDL_WINDOW_SHOWN);
		}
		fullscreen = not fullscreen;
	}
};
/* FULLSCREEN TEST */
