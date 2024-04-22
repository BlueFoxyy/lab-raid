#pragma once

#include <object/object.h>
#include <vector>
#include <string>

namespace Items {
	class Item {
	private:
		std::string itemName;
		const uint8_t cap;
		uint8_t count;
		std::unique_ptr<Objects::Object> instanceObject;
		std::unique_ptr<Objects::Object> inventoryObject;
	public:
		Item(
			const std::vector<std::string>& instanceTextureNames,
			const std::vector<std::string>& inventoryObject,
			const std::string& itemName,
			uint8_t cap,
			uint8_t count
		);
	};
}