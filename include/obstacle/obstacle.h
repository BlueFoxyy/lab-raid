#pragma once

#include <object/object.h>

namespace Obstacles {
	/// <summary>
	/// destructible environment objects
	/// </summary>
	class Obstacle {
	private:
		double durability;
		double penetration;
		bool indestructible;
	public:
		/// <summary>
		/// Check if the obstacle's durability is less than 0.
		/// </summary>
		/// <returns>Whether the obstacle was destroyed.</returns>
		bool destroyed() const noexcept;

		/// <summary>
		/// Applies damage to the obstacle.
		/// </summary>
		/// <param name="damage">The raw damage value.</param>
		/// <param name="penetrationLevel">The penetration level of the damage source.</param>
		void takeDamage(double damage, double penetrationLevel);
	};
}