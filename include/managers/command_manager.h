#pragma once

#include <command/command.h>
#include <input_handler.h>
#include <map>
#include <memory>

namespace Commands { class Command; }

enum class MouseButton : uint8_t;
/// <summary>
/// KeyBind structure for key bindings.
/// </summary>
struct KeyBind {
	static unsigned int KeyBindCount;
	int ID; // only used for sorting
	enum class Trigger { TAP, HOLD, RELEASE };
	std::map<SDL_Keycode, Trigger> keys;
	std::map<MouseButton, Trigger> buttons;
	KeyBind(const std::map<SDL_Keycode, Trigger>& keys, const std::map<MouseButton, Trigger> buttons):
		keys(keys), buttons(buttons) {
		ID = KeyBind::KeyBindCount++;
	}
	friend bool operator < (const KeyBind& a, const KeyBind& b) {
		return a.ID < b.ID;
	}
};

/// <summary>
/// Manages a map from key bindings to various functions.
/// e.g. player.move(), currentScene.set(mainMenu), or renderer.drawCone().
/// </summary>
class CommandManager {
private:
	std::map<KeyBind, std::shared_ptr<Commands::Command>> commandDB;
public:

	/// <summary>
	/// Registers a command for the specified key bind.
	/// </summary>
	/// <param name="keyBind">The key bind of this command.</param>
	/// <param name="command">The command to execute if the key bind is pressed.</param>
	/// <returns>Whether the command was successfully registered, fails if @param keyBind is already registered.</returns>
	bool registerCommand(KeyBind keyBind, std::shared_ptr<Commands::Command> command);

	/// <summary>
	/// Executes corresponding command if a key bind was matched.
	/// Note: beware of thread safety.
	/// </summary>
	void update() noexcept;
};