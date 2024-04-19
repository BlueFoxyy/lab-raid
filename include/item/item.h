#pragma once

#include <string>

namespace Items {
	class Item {
	private:
		std::string name;
		uint8_t count;
	public:
		void drop();
		void apply();
	};
}