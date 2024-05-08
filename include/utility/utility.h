#pragma once

#include "vector2d.h"
#include <SDL2/SDL.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <array>

/// <summary>
/// Helper function to normalize angle to [0, 2pi)
/// </summary>
/// <param name="angle">input angle</param>
/// <returns>normalized angle</returns>
float normalizeAngle(float angle) noexcept;

/// <summary>
/// Helper function to transform polar coordinates to cartesian coordinates.
/// </summary>
/// <param name="radius">input radius</param>
/// <param name="theta">input angle (radians)</param>
/// <returns>the transformed cartesian coordinates</returns>
Vector2D polarToCartesian(float radius, float theta);

/// <summary>
/// Checks if two rectangles collides.
/// </summary>
/// <param name="rect1">First rect.</param>
/// <param name="angle1">The rotation of the first rect.</param>
/// <param name="rect2">Second rect.</param>
/// <param name="angle2">The rotation of the second rect.</param>
/// <returns>If the rectangles collides.</returns>
bool rectCollide(const SDL_FRect& rect1, float angle1, const SDL_FRect& rect2, float angle2);