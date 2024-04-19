#pragma once

#include <utility/pointer_wrappers.h>
#include <geometry/vector2d.h>
#include <texture_handler.h>
#include <view/view.h>
#include <SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Views {
	class View;
}
class TextureHandler;

namespace Objects {
	const int TEXTURE_NOT_SET = -1;

	/// <summary>
	/// Object type for all renderable objects in the world
	/// note: the texture won't be created until loaded into the renderer.
	/// </summary>
	class Object {
		friend class TextureHandler;
	private:
		std::unordered_map<std::string, int> textureIdMap;
		std::vector<std::weak_ptr<SDL_Texture>> textures;
		int currentTextureId;
		
		float angle; // stored as radians
		SDL_RendererFlip flipFlag;
		uint8_t alpha;
		Vector2D position; // actual position in the world
		Vector2D dimension; // height and width
		const Views::View* view;
	public:

		/// <summary>
		/// Constructs a new object.
		/// </summary>
		/// <param name="textureNames">The list of texture names.</param>
		/// <param name="_view">The viewport of the object.</param>
		/// <param name="_position">Initial position. (x, y)</param>
		/// <param name="_dimension">Initial Dimension. (width, height)</param>
		Object(
			const std::vector<std::string>& textureNames,
			const Views::View* _view, 
			const Vector2D& _position, 
			const Vector2D& _dimension
		);

		virtual ~Object() = default;

		/// <summary>
		/// Returns the angle of the object in radians.
		/// The returned angle will be in [0, 2pi), with
		/// 0 set at positive x direction, and going counter-clockwise.
		/// </summary>
		/// <returns>The angle which the object is facing.</returns>
		float getAngle(void) const noexcept;
		
		/// <summary>
		/// Sets rotation angle to @param newAngle radians.
		/// </summary>
		/// <param name="newAngle">The new angle to set to. (in radians)</param>
		void setAngle(float newAngle) noexcept;

		/// <summary>
		/// Rotates the object by @param diffAngle radians in 
		/// the counter-clockwise direction.
		/// </summary>
		/// <param name="diffAngle">Rotation angle.</param>
		void rotate(float diffAngle) noexcept;

		/// <summary>
		/// Returns the flip flag used by SDL.
		/// </summary>
		/// <returns>A SDL_RendererFlip flag.</returns>
		SDL_RendererFlip getFlipFlag(void) const noexcept;

		/// <summary>
		/// Gets the position of the object.
		/// </summary>
		/// <returns>The object's location.</returns>
		Vector2D getPosition(void) const noexcept;

		/// <summary>
		/// Gets the dimension of the object.
		/// </summary>
		/// <returns>The object's dimension.</returns>
		Vector2D getDimension(void) const noexcept;

		/// <summary>
		/// Gets the alpha value of the object. (Opacity/Transparency)
		/// </summary>
		/// <returns>The object's alpha value.</returns>
		uint8_t getAlpha(void) const noexcept;

		/// <summary>
		/// Sets the alpha value of the object. (Opacity/Transparency)
		/// </summary>
		/// <param name="newAlpha">New alpha value.</param>
		void setAlpha(uint8_t newAlpha) noexcept;
		
		/// <summary>
		/// Moves the object by the translate vector.
		/// </summary>
		/// <param name="translate">The offset vector to move by.</param>
		void move(const Vector2D& translate) noexcept;

		/// <summary>
		/// Stretches the object's width by @param ratio.
		/// </summary>
		/// <param name="ratio">Stretch ratio.</param>
		void stretchX(float ratio) noexcept;

		/// <summary>
		/// Stretches the object's height by @param ratio.
		/// </summary>
		/// <param name="ratio">Stretch ratio.</param>
		void stretchY(float ratio) noexcept;

		/// <summary>
		/// Stretches both the object's width and height by @param ratio.
		/// </summary>
		/// <param name="ratio">Stretch ratio.</param>
		void stretch(float ratio) noexcept;

		/// <summary>
		/// Flips the object horizontally.
		/// </summary>
		void flipHorizontal(void) noexcept;

		/// <summary>
		/// Flips the object vertically.
		/// </summary>
		/// <param name=""></param>
		void flipVertical(void) noexcept;
		
		/* TEXTURES */
		
		/// <summary>
		/// Set to next texture, texture ID wraps around.
		/// </summary>
		/// <returns>New texture ID.</returns>
		int nextTexture(void) noexcept;

		/// <summary>
		/// Set to previous texture, texture ID wraps around.
		/// </summary>
		/// <returns>New texture ID.</returns>
		int previousTexture(void) noexcept;

		/// <summary>
		/// Sets texture to @param textureId.
		/// </summary>
		/// <param name="textureId">The ID of the texture to be set. Should be in [0, textureCount).</param>
		/// <returns>New texture ID.</returns>
		int setTexture(int textureId) noexcept;

		/// <summary>
		/// Sets texture to @param textureName.
		/// </summary>
		/// <param name="textureName">The name of the texture to be set.</param>
		/// <returns>New texture ID.</returns>
		int setTexture(const std::string& textureName) noexcept;

		/// <summary>
		/// Gets the number of textures this object has.
		/// </summary>
		/// <returns>Numbeer of textures</returns>
		size_t getTextureCount(void) const noexcept;
		
		/// <summary>
		/// Gets current texture.
		/// </summary>
		/// <returns>The current texture the object is using.</returns>
		SDL_Texture* getTexture(void) const noexcept;
		/* TEXTURES */
		
		/// <summary>
		/// Make the object face @param position coordinates.
		/// </summary>
		/// <param name="position">The coordinate of where the object should look at.</param>
		virtual void lookAt(const Vector2D& position) noexcept;

		/// <summary>
		/// Gets render rectangle for rendering.
		/// </summary>
		/// <returns>The SDL_FRect raw pointer for rendering.</returns>
		SDL_FRect* getRenderRect(void) const noexcept;

		/// <summary>
		/// Updates the object state.
		/// </summary>
		virtual void update() noexcept;

		// debug
		void debug(void) const;
	};
}