#pragma once

#include <utility/vector2d.h>
#include <SDL2/SDL.h>
#include <functional>
#include <map>
#include <memory>

enum class MouseButton : uint8_t {
	LEFT	= SDL_BUTTON_LEFT,
	MIDDLE	= SDL_BUTTON_MIDDLE,
	RIGHT	= SDL_BUTTON_RIGHT,
	X1		= SDL_BUTTON_X1,
	X2		= SDL_BUTTON_X2
};

/// <summary>
/// This is a global singleton class of handling user inputs.
/// Wrapper class of SDL_PollEvent and events handling.
/// </summary>
class InputHandler {
private:
//	friend class CommandManager;

	struct KeyState {
		enum { PRESSED, RELEASED, NONE } toggle;
		enum { UP, DOWN } hold;
		uint32_t holdStart; // The tick this key was first held down.
		KeyState() :
			toggle(NONE),
			hold(UP),
			holdStart(0) {}
		void toggleDown(void) noexcept {
			if (hold == UP) {
				toggle = PRESSED;
				holdStart = SDL_GetTicks();
			}
			hold = DOWN;
		}
		void toggleUp(void) noexcept {
			if (hold == DOWN) {
				toggle = RELEASED;
			}
			hold = UP;
		}
		uint32_t getHoldTime(void) const noexcept {
			if (hold == DOWN)
				return SDL_GetTicks() - holdStart;
			return 0;
		}
	};
	std::map<SDL_Keycode, KeyState> keyStateDB;
	std::map<MouseButton, KeyState> mouseButtonStateDB;

	InputHandler();
public:
	InputHandler(const InputHandler&) = delete;
	void operator = (const InputHandler&) = delete;

	static InputHandler& getInstance(void) noexcept;


	// Keyboard functions

	/// <summary>
	/// Checks if a key is pressed. (SDL_KeyDown)
	/// Is only true when the key was not held down.
	/// </summary>
	/// <param name="key">SDL_Keycode key value.</param>
	/// <returns>Whether the key was pressed.</returns>
	bool pollKeyPress(SDL_Keycode key) noexcept;
	
	/// <summary>
	/// Checks if a key is released. (SDL_KeyUp)
	/// </summary>
	/// <param name="key">SDL_Keycode key value.</param>
	/// <returns>Whether the key was released.</returns>
	bool pollKeyRelease(SDL_Keycode key) noexcept;

	/// <summary>
	/// Checks if a key is held down. (SDL_KeyDown)
	/// </summary>
	/// <param name="key">SDL_Keycode key value.</param>
	/// <returns>Whether the key was held down.</returns>
	bool isKeyDown(SDL_Keycode key) const noexcept;

	/// <summary>
	/// Checks if a key is not held down.
	/// </summary>
	/// <param name="key">SDL_Keycode key value.</param>
	/// <returns>Whether the key was not held down.</returns>
	bool isKeyUp(SDL_Keycode key) const noexcept;

	/// <summary>
	/// Gets the time a key was held down in SDL_Ticks.
	/// </summary>
	/// <returns>How long the key was held down.</returns>
	uint32_t holdTime(SDL_Keycode key) const noexcept;


	// Mouse functions

	bool pollButtonPress(MouseButton button) noexcept;
	bool pollButtonRelease(MouseButton button) noexcept;
	bool isButtonDown(MouseButton button) const noexcept;
	bool isButtonUp(MouseButton button) const noexcept;
	uint32_t holdTime(MouseButton button) const noexcept;
	Vector2D getMousePosition(void) const noexcept;

	
	// Event Receivers
	
	void receiveEvent(SDL_KeyboardEvent keyboardEvent) noexcept;
	void receiveEvent(SDL_MouseButtonEvent mouseButtonEvent) noexcept;
	//void receiveEvent(SDL_MouseMotionEvent mouseMotionEvent) noexcept;
	//void receiveEvent(SDL_MouseWheelEvent mouseWheelEvent) noexcept;
};