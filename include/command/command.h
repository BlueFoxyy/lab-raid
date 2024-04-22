#pragma once

#include <memory>

namespace Commands {

	/// <summary>
	/// Commands base abstract class.
	/// </summary>
	class Command {
	public:
		virtual ~Command() {};
		virtual void execute(void) {};
	};
}