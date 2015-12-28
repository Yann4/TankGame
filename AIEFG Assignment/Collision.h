#pragma once
#include "ObjectTypes.h"
#include <math.h>

namespace Collision
{
	struct BoundingBox
	{
		float x, y;
		float halfWidth, halfHeight;
		
		BoundingBox() :x(0), y(0), halfWidth(0), halfHeight(0)
		{}

		BoundingBox(float x, float y, float hWidth, float hHeight) : x(x), y(y), halfHeight(hHeight), halfWidth(hWidth)
		{}
	};

	struct MTV
	{
		position direction;
		float magnitude;

		MTV() : direction(position(0, 0)), magnitude(0)
		{}

		MTV(position direction, float mag) : direction(direction), magnitude(mag)
		{}
	};

	bool collision(BoundingBox self, BoundingBox other, MTV& mtv);

	bool testAxis(position axis, float minA, float maxA, float minB, float maxB, MTV& mtv);
}