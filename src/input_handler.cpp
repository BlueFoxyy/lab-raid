#include <input_handler.h>

InputHandler::InputHandler() {}

InputHandler& InputHandler::getInstance(void) noexcept {
	static InputHandler inputHandler;
	return inputHandler;
}

bool InputHandler::pollKeyPress(SDL_Keycode key) noexcept {
	KeyState& state = keyStateDB[key];
	if (state.toggle == KeyState::PRESSED) {
		state.toggle = KeyState::NONE;
		return true;
	}
	return false;
}
bool InputHandler::pollKeyRelease(SDL_Keycode key) noexcept {
	KeyState& state = keyStateDB[key];
	if (state.toggle == KeyState::RELEASED) {
		state.toggle = KeyState::NONE;
		return true;
	}
	return false;
}
bool InputHandler::isKeyDown(SDL_Keycode key) const noexcept {
	try {
		return keyStateDB.at(key).hold == KeyState::DOWN;
	} catch (...) {
		return false;
	}
}
bool InputHandler::isKeyUp(SDL_Keycode key) const noexcept {
	try {
		return keyStateDB.at(key).hold == KeyState::UP;
	} catch (...) {
		return false;
	}
}
uint32_t InputHandler::holdTime(SDL_Keycode key) const noexcept {
	try {
		const KeyState& keyState = keyStateDB.at(key);
		return keyState.getHoldTime();
	} catch (...) {
		return 0;
	}
}

bool InputHandler::pollButtonPress(MouseButton button) noexcept {
	KeyState& state = mouseButtonStateDB[button];
	if (state.toggle == KeyState::PRESSED) {
		state.toggle = KeyState::NONE;
		return true;
	}
	return false;
}
bool InputHandler::pollButtonRelease(MouseButton button) noexcept {
	KeyState& state = mouseButtonStateDB[button];
	if (state.toggle == KeyState::RELEASED) {
		state.toggle = KeyState::NONE;
		return true;
	}
	return false;
}
bool InputHandler::isButtonDown(MouseButton button) const noexcept {
	try {
		return mouseButtonStateDB.at(button).hold == KeyState::DOWN;
	} catch (...) {
		return false;
	}
}
bool InputHandler::isButtonUp(MouseButton button) const noexcept {
	try {
		return mouseButtonStateDB.at(button).hold == KeyState::UP;
	} catch (...) {
		return false;
	}
}
uint32_t InputHandler::holdTime(MouseButton button) const noexcept {
	try {
		const KeyState& state = mouseButtonStateDB.at(button);
		return state.getHoldTime();
	} catch (...) {
		return 0;
	}
}
Vector2D InputHandler::getMousePosition(void) const noexcept {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return Vector2D(mouseX, mouseY);
}


void InputHandler::receiveEvent(SDL_KeyboardEvent keyboardEvent) noexcept {
	SDL_Keycode key = keyboardEvent.keysym.sym;
	KeyState& state = keyStateDB[key];
	switch (keyboardEvent.type) {
	case SDL_KEYDOWN:
		state.toggleDown();
		break;
	case SDL_KEYUP:
		state.toggleUp();
		break;
	}
}
void InputHandler::receiveEvent(SDL_MouseButtonEvent mouseButtonEvent) noexcept {
	MouseButton button = static_cast<MouseButton>(mouseButtonEvent.button);
	KeyState& state = mouseButtonStateDB[button];
	switch (mouseButtonEvent.type) {
	case SDL_MOUSEBUTTONDOWN:
		state.toggleDown();
		break;
	case SDL_MOUSEBUTTONUP:
		state.toggleUp();
	}
}