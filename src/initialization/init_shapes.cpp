#include <init.h>

void Global::initShapes(void) {
	using namespace RuntimeShapes;

	yellowCircle = std::make_shared<Shapes::Circle>(
		playerCamera.get(),
		Vector2D{ -150, -150 },
		50,
		SDL_Color{ 127, 127, 0, 127 }
	);
	greenCircle = std::make_shared<Shapes::Circle>(
		playerCamera.get(),
		Vector2D{ -150, 150 },
		50,
		SDL_Color{ 0, 127, 0, 127 }
	);
	blueCircle = std::make_shared<Shapes::Circle>(
		playerCamera.get(),
		Vector2D{ 150, -150 },
		50,
		SDL_Color{ 0, 0, 127, 127 }
	);
	redCircle = std::make_shared<Shapes::Circle>(
		playerCamera.get(),
		Vector2D{ 150, 150 },
		50,
		SDL_Color{ 127, 0, 0, 127 }
	);
	purpleCircle = std::make_shared<Shapes::Circle>(
		playerCamera.get(),
		Vector2D{ 0, 0 },
		180,
		SDL_Color{ 220, 36, 200, 127 }
	);
	hollowCircle1 = std::make_shared<Shapes::HollowCircle>(
		playerCamera.get(),
		Vector2D{ 0, 0 },
		100,
		10
	);
	line1 = std::make_shared<Shapes::Line>(
		playerCamera.get(),
		Vector2D{ -100, -200 },
		Vector2D{ -100, 200 },
		10
	);
	line2 = std::make_shared<Shapes::Line>(
		playerCamera.get(),
		Vector2D{ 100, -200 },
		Vector2D{ 100, 200 },
		10
	);
	line3 = std::make_shared<Shapes::Line>(
		playerCamera.get(),
		Vector2D{ -200, -100 },
		Vector2D{ 200, -100 },
		10
	);
	line4 = std::make_shared<Shapes::Line>(
		playerCamera.get(),
		Vector2D{ -200, 100 },
		Vector2D{ 200, 100 },
		10
	);
	hudCircle = std::make_shared<Shapes::Circle>(
		hudView.get(),
		hudView->getDimension() + Vector2D(-50, -50),
		50,
		SDL_Color{ 0xFF, 0xFF, 0xFF, 0x7F }
	);
	// CROSSHAIR
	crosshairLine1 = std::make_shared<Shapes::Line>(
		hudView.get(),
		Vector2D{ 0, -31 },
		Vector2D{ 0, 31 },
		2
	);
	crosshairLine2 = std::make_shared<Shapes::Line>(
		hudView.get(),
		Vector2D{ -31, 0 },
		Vector2D{ 31, 0 },
		2
	);
	crosshairCircle1 = std::make_shared<Shapes::HollowCircle>(
		hudView.get(),
		Vector2D{ 0, 0 },
		13.5,
		2
	);
}