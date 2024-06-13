#pragma once

#include <render_object_base.h>
#include <utility/utility.h>
#include <utility/pointer_wrappers.h>
#include <utility/vector2d.h>
#include <utility/selection_manager.h>
#include <texture/texture_handler.h>
#include <view/view.h>
#include <config.h>
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <stdexcept>

namespace Views { class View; }
class TextureHandler;

namespace Objects {

	// TODO: add 'shapes' field to `Objects::Object`

	/// <summary>
	/// Object type for all renderable objects in the world
	/// note: the texture won't be created until loaded into the renderer.
	/// </summary>
	class Object : public RenderObjectBase {
		friend class TextureHandler;
	private:
		SelectionManager<SDL_Texture*> textures;
		bool visible;

		float angle; // stored as radians
		SDL_RendererFlip flipFlag;
//		SDL_Color colorMask; // color mod mask
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
		/// Gets the render angle of the object.
		/// </summary>
		/// <returns>The render angle of the object</returns>
		float getRenderAngle(void) const noexcept;
		
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
		/// Sets the position of the object.
		/// </summary>
		void setPosition(const Vector2D& newPosition) noexcept;

		/// <summary>
		/// Gets the dimension of the object.
		/// </summary>
		/// <returns>The object's dimension.</returns>
		Vector2D getDimension(void) const noexcept;

		/// <summary>
		/// Sets the dimension of the object. Throws 'std::invalid_argument' if one of the dimensions is negative.
		/// </summary>
		void setDimension(const Vector2D& newDimension);
		
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
		void flipVertical(void) noexcept;

		/// <summary>
		/// Sets the object's visibility.
		/// </summary>
		/// <param name="visibility">The object's visibility.</param>
		void setVisibility(bool visibility) noexcept;
		
		/// <summary>
		/// Gets the object's visibility.
		/// </summary>
		/// <returns>The object's visibility.</returns>
		bool getVisibility(void) const noexcept; 

		/// @brief Check if this object collides with `other' object.
		/// @param other The other object.
		/// @return If collided.
		bool collideWith(const Object& other) const noexcept;

		/* TEXTURES */
		
		/// <summary>
		/// Set to next texture, texture ID wraps around.
		/// </summary>
		void nextTexture(void) noexcept;

		/// <summary>
		/// Set to previous texture, texture ID wraps around.
		/// </summary>
		void previousTexture(void) noexcept;

		/// <summary>
		/// Sets texture to @param textureId.
		/// </summary>
		/// <param name="textureId">The ID of the texture to be set. Should be in [0, textureCount).</param>
		void setTexture(int textureId) noexcept;

		/// <summary>
		/// Gets the number of textures this object has.
		/// </summary>
		/// <returns>Numbeer of textures.</returns>
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
		/// <returns>The SDL_FRect for rendering.</returns>
		SDL_FRect getRenderRect(void) const noexcept;
		//Vector2D getRenderRelativePosition(Vector2D renderPosition) const noexcept;

		/// <summary>
		/// Updates the object state.
		/// </summary>
		virtual void update(void) noexcept;

		// debug
		void debug(void) const noexcept override;
	};
}