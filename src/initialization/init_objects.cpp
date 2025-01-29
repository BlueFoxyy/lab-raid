#include <init.h>

void Global::initObjects(void) {
	using namespace RuntimeObjects;
	player = std::make_shared<Objects::Object>(
		std::vector<std::string>{ "player" },
		playerCamera.get(),
		Vector2D{ 0, 0 },
		Vector2D{ 100, 100 }
	);
	arrow1 = std::make_shared<Objects::Object>(
		std::vector<std::string>{ "arrow" },
		playerCamera.get(),
		Vector2D{ -150, -150 },
		Vector2D{ 50, 37.5f }
	);
	arrow2 = std::make_shared<Objects::Object>(
		std::vector<std::string>{ "arrow" },
		playerCamera.get(),
		Vector2D{ 150, 150 },
		Vector2D{ 50, 37.5f }
	);
	hudArrow = std::make_shared<Objects::Object>(
		std::vector<std::string>{ "arrow" },
		hudView.get(),
		Vector2D{ 0, 12.5f },
		Vector2D{ 100, 75 }
	);
	camera = std::make_shared<Objects::Object>(
		std::vector<std::string>{}, // no texture
		playerCamera.get(),
		Vector2D{ 0, 0 },
		Vector2D{ 0, 0 }
	);

	currentObjectSelection.add(player);
	currentObjectSelection.add(arrow1);
	currentObjectSelection.add(arrow2);
}