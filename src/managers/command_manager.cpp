#include <managers/command_manager.h>

unsigned int KeyBind::KeyBindCount;

bool CommandManager::registerCommand(
	KeyBind keyBind,
	std::shared_ptr<Commands::Command> command
) {
	if (commandDB.find(keyBind) != commandDB.end())
		return false;
	commandDB[keyBind] = command;
//	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "CommandManager::registerCommand(): Registered command.");
	return true;
}

void CommandManager::update(void) noexcept {
	std::map<SDL_Keycode, bool> keyPressedStates;
	std::map<SDL_Keycode, bool> keyReleasedStates;
	std::map<MouseButton, bool> buttonPressedStates;
	std::map<MouseButton, bool> buttonReleasedStates;
	for (auto& [keyBind, commandPtr] : commandDB) {
		bool match = true;
		for (auto& [key, trigger] : keyBind.keys) if (match) {
			switch (trigger) {
			case KeyBind::Trigger::TAP:
				if (keyPressedStates.find(key) == keyPressedStates.end())
					keyPressedStates[key] = InputHandler::getInstance().pollKeyPress(key);
				if (not keyPressedStates[key]) {
					match = false;
					break;
				}
				break;
			case KeyBind::Trigger::RELEASE:
				if (keyReleasedStates.find(key) == keyReleasedStates.end())
					keyReleasedStates[key] = InputHandler::getInstance().pollKeyRelease(key);
				if (not keyReleasedStates[key]) {
					match = false;
					break;
				}
				break;
			case KeyBind::Trigger::HOLD:
				if (not InputHandler::getInstance().isKeyDown(key)) {
					match = false;
					break;
				}
				break;
			}
		}
		for (auto& [button, trigger] : keyBind.buttons) if (match) {
			switch (trigger) {
			case KeyBind::Trigger::TAP:
				if (buttonPressedStates.find(button) == buttonPressedStates.end())
					buttonPressedStates[button] = InputHandler::getInstance().pollButtonPress(button);
				if (not buttonPressedStates[button]) {
					match = false;
					break;
				}
				break;
			case KeyBind::Trigger::RELEASE:
				if (buttonReleasedStates.find(button) == buttonReleasedStates.end())
					buttonReleasedStates[button] = InputHandler::getInstance().pollButtonRelease(button);
				if (not buttonReleasedStates[button]) {
					match = false;
					break;
				}
				break;
			case KeyBind::Trigger::HOLD:
				if (not InputHandler::getInstance().isButtonDown(button)) {
					match = false;
					break;
				}
				break;
			}
		}
		if (match) {
			commandPtr->execute({});
		}

		/*
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "KeyBind:");
		if (SDL_LogGetPriority(SDL_LOG_CATEGORY_APPLICATION) <= SDL_LOG_PRIORITY_DEBUG) {
			for (auto& [key, trigger] : keyBind.keys) {
				SDL_LogDebug(
					SDL_LOG_CATEGORY_APPLICATION,
					"Keyboard: \'%s\': %s",
					SDL_GetKeyName(key),
					trigger == KeyBind::Trigger::HOLD ? "HOLD" :
					trigger == KeyBind::Trigger::RELEASE ? "RELEASE" : "TAP"
				);
			}
			for (auto& [button, trigger] : keyBind.buttons) {
				SDL_LogDebug(
					SDL_LOG_CATEGORY_APPLICATION,
					"Mouse: \'%u\': %s",
					static_cast<uint8_t>(button),
					trigger == KeyBind::Trigger::HOLD ? "HOLD" :
					trigger == KeyBind::Trigger::RELEASE ? "RELEASE" : "TAP"
				);
			}
		}
		SDL_LogDebug(
			SDL_LOG_CATEGORY_APPLICATION,
			"match: %s",
			match ? "true" : "false"
		);
		*/
	}
}