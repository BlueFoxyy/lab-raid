#pragma once

#include <object/object.h>
#include <memory>

namespace Commands {

	/// <summary>
	/// Commands base abstract class.
	/// </summary>
	class Command {
	public:
		virtual ~Command() = delete;
		virtual void execute(std::weak_ptr<Objects::Object> object) = delete;
	};
}