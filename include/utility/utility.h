#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "vector2d.h"

/// <summary>
/// Helper function to normalize angle to [0, 2pi)
/// </summary>
/// <param name="angle">input angle</param>
/// <returns>normalized angle</returns>
float normalizeAngle(float angle) noexcept;

/// <summary>
/// Helper function to to transform polar coordinates to cartesian coordinates.
/// </summary>
/// <param name="radius">input radius</param>
/// <param name="theta">input angele (radians)</param>
/// <returns>the transformed cartesian coordinates</returns>
Vector2D polarToCartesian(float radius, float theta);