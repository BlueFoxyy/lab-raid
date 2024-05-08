#include <object/object.h>

namespace Objects {
	Object::Object(
		const std::vector<std::string>& textureNames,
		const Views::View* _view,
		const Vector2D& _position,
		const Vector2D& _dimension
	) :
		visible(true),
		angle(0),
		/*	rect(std::unique_ptr<SDL_FRect>(new SDL_FRect({
				_position.getX(),
				_position.getY(),
				_dimension.getX(),
				_dimension.getY()
			}))), */
//		colorMask({255, 255, 255, 255}),
		flipFlag(SDL_FLIP_NONE),
		position(_position),
		dimension(_dimension),
		view(_view) {
		for (const std::string& textureName : textureNames) {
			textures.add(TextureHandler::getInstance().getTexture({}, textureName));
		}
	}
	
	float Object::getAngle(void) const noexcept {
		return angle;
	}

	float Object::getRenderAngle(void) const noexcept {
		return angle - view->getAngle();
	}

	SDL_RendererFlip Object::getFlipFlag(void) const noexcept {
		return flipFlag;
	}

	Vector2D Object::getPosition(void) const noexcept {
		return position;
	}

	Vector2D Object::getDimension(void) const noexcept {
		return dimension;
	}

	void Object::move(const Vector2D& translate) noexcept {
		position += translate;
	}

	void Object::setAngle(float newAngle) noexcept {
		angle = normalizeAngle(newAngle);
	}

	void Object::rotate(float diffAngle) noexcept {
		angle = normalizeAngle(angle + diffAngle);
	}

	void Object::stretchX(float ratio) noexcept {
		SDL_assert(ratio > 0);
		dimension = Vector2D(dimension.getX() * ratio, dimension.getY());
	}

	void Object::stretchY(float ratio) noexcept {
		SDL_assert(ratio > 0);
		dimension = Vector2D(dimension.getX(), dimension.getY() * ratio);
	}

	void Object::stretch(float ratio) noexcept {
		SDL_assert(ratio > 0);
		dimension *= ratio;
	}

	void Object::flipHorizontal(void) noexcept {
		flipFlag = static_cast<SDL_RendererFlip>(flipFlag ^ SDL_FLIP_HORIZONTAL);
	}

	void Object::flipVertical(void) noexcept {
		flipFlag = static_cast<SDL_RendererFlip>(flipFlag ^ SDL_FLIP_VERTICAL);
	}

	void Object::setVisibility(bool visibility) noexcept {
		visible = visibility;
	}

	bool Object::getVisibility(void) const noexcept {
		return visible;
	}

	void Object::lookAt(const Vector2D& targetPosition) noexcept {
		Vector2D offset = targetPosition - this->position;
		offset = offset.norm();
		angle = -atan2(offset.getY(), offset.getX());
		angle = normalizeAngle(angle);
		
		/*
		SDL_LogDebug(
			SDL_LOG_CATEGORY_APPLICATION,
			"LookAt(): (%lf, %lf) - (%lf, %lf) = (%lf, %lf) : %lf",
			targetPosition.getX(), targetPosition.getY(),
			this->position.getX(), this->position.getY(),
			offset.getX(), offset.getY(),
			angle
		);
		*/
	}

	SDL_FRect Object::getRenderRect(void) const noexcept {
		return view->getRect(*this);
	}

//	SDL_Color Object::getColorMask(void) const noexcept {
//		return colorMask;
//	}

//	void Object::setColorMask(const SDL_Color& newColorMask) noexcept {
//		colorMask = newColorMask;
//	}

	bool Object::collideWith(const Object& other) const noexcept {
		return rectCollide(
			this->getRenderRect(), this->getRenderAngle(),
			other.getRenderRect(), other.getRenderAngle()
		);
	}

	void Object::nextTexture(void) noexcept {
		textures.next();
	}

	void Object::previousTexture(void) noexcept {
		textures.prev();
	}

	void Object::setTexture(int textureId) noexcept {
		textures.set(textureId);
	}

	size_t Object::getTextureCount(void) const noexcept {
		return textures.size();
	}

	SDL_Texture* Object::getTexture(void) const noexcept {
		try {
			return textures.get();
		} catch (...) {
			return nullptr;
		}
	}

	void Object::debug(void) const noexcept {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Object:");
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Address: %p", this);
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Position: (%lf, %lf)",
			position.getX(), position.getY()
		);
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Dimension: (%lf, %lf)",
			dimension.getX(), dimension.getY()
		);
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Angle: %lf rad",
			angle
		);
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "HorizontalFlip: %s",
			(flipFlag & SDL_FLIP_HORIZONTAL ? "true" : "false")
		);
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "VerticalFlip: %s",
			(flipFlag & SDL_FLIP_VERTICAL ? "true" : "false")
		);
	}

	/*
	Vector2D Object::getRenderRelativePosition(Vector2D renderPosition) const noexcept {
		auto renderRect = getRenderRect();
		Vector2D objectRenderPosition = { renderRect.x, renderRect.y };
		Vector2D diffVector = renderPosition - objectRenderPosition;
		diffVector = {
			diffVector.getX() * view->getDimension().getX() / view->getZoom() / Config::screenWidth,
			diffVector.getY() * view->getDimension().getY() / view->getZoom() / Config::screenHeight
		};
		diffVector -= getDimension() / 2;
		diffVector.rotate(view->getAngle());
		return diffVector;
	}
	*/

	void Object::update(void) noexcept {}
}