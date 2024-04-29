#pragma once

#include <command_manager.h>
#include <memory>

class CommandManager;

namespace Commands {

	/// <summary>
	/// Commands base abstract class.
	/// </summary>
	class Command {
	protected:
		class ExecuteKey {
			friend class CommandManager;
		private:
			ExecuteKey() = default;
			ExecuteKey(const ExecuteKey&) = default;
		};
	public:
		virtual ~Command() {};
		virtual void execute(ExecuteKey) {};
	};
}