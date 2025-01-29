#include <init.h>

static float playerSpeed = 500;

class MoveUpCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(0, -playerSpeed * Global::tickManager.getDiffTick()).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveDownCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(0, playerSpeed * Global::tickManager.getDiffTick()).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveLeftCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(-playerSpeed * Global::tickManager.getDiffTick(), 0).rotate(
				Global::playerCamera->getAngle()
			)
		);
	}
};
class MoveRightCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::currentObjectSelection.get()->move(
			Vector2D(playerSpeed * Global::tickManager.getDiffTick(), 0).rotate(
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
float zoomMultiplier = 1.0f;
float zoomSpeed = 1.5f;

class ZoomInCommand : public Commands::Command {
public:
	// for testing purposes
	static void command() noexcept {
		zoomMultiplier = std::min(zoomMultiplier + zoomSpeed * Global::tickManager.getDiffTick(), 4.0f);
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
		zoomMultiplier = std::max(zoomMultiplier - zoomSpeed * Global::tickManager.getDiffTick(), 0.25f);
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
		Global::playerCamera->rotate(rotateSpeed * Global::tickManager.getDiffTick());
	}
};

class RotateCameraCounterClockwiseCommand : public Commands::Command {
public:
	void execute(const ExecuteKey& key) noexcept override {
		Global::playerCamera->rotate(-rotateSpeed * Global::tickManager.getDiffTick());
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
			Vector2D{ 40 , gunOffset }.rotate(
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
void Global::initCommands() {
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