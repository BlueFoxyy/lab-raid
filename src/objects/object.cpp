#include <object/object.h>
#include <geometry/vector2d.h>
#include <texture_handler.h>
#include <SDL.h>
#include <string>
#include <memory>
#include <stdexcept>
#include <format>


namespace Objects {
	/// <summary>
	/// Helper function to normalize angle to [0, 360)
	/// </summary>
	/// <param name="angle">input angle</param>
	/// <returns>normalized angle</returns>
	static float normalizeAngle(float angle) noexcept {
		if (angle < 0) {
			angle += -ceil(angle / (2 * M_PI)) * (2 * M_PI);
		} else {
			angle -= floor(angle / (2 * M_PI)) * (2 * M_PI);
		}
		return angle;
	}

	Object::Object(
		const std::vector<std::string>& textureNames,
		const Views::View* _view, 
		const Vector2D& _position, 
		const Vector2D& _dimension
	) :	
		textures(textureNames.size()),
		angle(0),
		/*	rect(std::unique_ptr<SDL_FRect>(new SDL_FRect({
				_position.getX(),
				_position.getY(),
				_dimension.getX(),
				_dimension.getY()
			}))), */
		alpha(255),
		flipFlag(SDL_FLIP_NONE),
		position(_position),
		dimension(_dimension),
		view(_view) {
		for (int textureId = 0; textureId < (int)textureNames.size(); textureId++) {
			const std::string& textureName = textureNames[textureId];
			// assert the texture isn't already registered
			SDL_assert(textureIdMap.find(textureName) == textureIdMap.end());
			textureIdMap[textureName] = textureId;
		}
		currentTextureId = TEXTURE_NOT_SET;
	}
	
	float Object::getAngle(void) const noexcept {
		return angle;
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

	void Object::lookAt(const Vector2D& targetPosition) noexcept {
		Vector2D offset = targetPosition - this->position;
		angle = -atan2(offset.getY(), offset.getX());
		angle = normalizeAngle(angle);
		
		SDL_LogInfo(
			SDL_LOG_CATEGORY_APPLICATION,
			"LookAt(): (%lf, %lf) - (%lf, %lf) = (%lf, %lf) : %lf",
			targetPosition.getX(), targetPosition.getY(),
			this->position.getX(), this->position.getY(),
			offset.getX(), offset.getY(),
			angle
		);
	}

	SDL_FRect* Object::getRenderRect(void) const noexcept {
		return view->getRect(*this);
	}

	void Object::setAlpha(uint8_t newAlpha) noexcept {
		SDL_assert(0 <= newAlpha and newAlpha <= 255);
		alpha = newAlpha;
	}

	uint8_t Object::getAlpha(void) const noexcept {
		return alpha;
	}

	int Object::nextTexture(void) noexcept {
		if (currentTextureId == TEXTURE_NOT_SET)
			return TEXTURE_NOT_SET;
		currentTextureId = (currentTextureId + 1) % getTextureCount();
		return currentTextureId;
	}

	int Object::previousTexture(void) noexcept {
		if (currentTextureId == TEXTURE_NOT_SET)
			return TEXTURE_NOT_SET;
		currentTextureId = currentTextureId - 1;
		if (currentTextureId < 0) currentTextureId += getTextureCount();
		return currentTextureId;
	}

	int Object::setTexture(int textureId) noexcept {
		if (currentTextureId == TEXTURE_NOT_SET)
			return TEXTURE_NOT_SET;
		if (not (0 <= textureId and textureId < getTextureCount()))
			return currentTextureId;
		currentTextureId = textureId;
		return currentTextureId;
	}

	int Object::setTexture(const std::string& textureName) noexcept {
		if (currentTextureId == TEXTURE_NOT_SET)
			return TEXTURE_NOT_SET;
		try {
			currentTextureId = textureIdMap.at(textureName);
		} catch (std::out_of_range& e) {
			SDL_LogVerbose(
				SDL_LOG_CATEGORY_APPLICATION,
				"Object::setTexture(): Failed to set texture to '%s'.",
				textureName
			);
			return currentTextureId;
		}
		return currentTextureId;
	}

	size_t Object::getTextureCount(void) const noexcept {
		return textureIdMap.size();
	}

	SDL_Texture* Object::getTexture(void) const noexcept {
		if (currentTextureId == TEXTURE_NOT_SET)
			return nullptr;
		if (auto tmp = textures[currentTextureId].lock())
			return tmp.get();
		return nullptr;
	}

	void Object::debug(void) const {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Object:");
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Address: %p", this);
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Textures:");
		for (const auto& [textureName, textureId] : textureIdMap) {
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\tID = %d, name = '%s'",
				textureId, textureName
			);
		}
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
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Alpha: %lf",
			getAlpha()
		);
	}

	void Object::update() noexcept {

	}
}