#include "Collision.h"

bool Collision::collision(BoundingBox self, BoundingBox other, MTV& mtv)
{
	bool collision = false;
	mtv = MTV(position(0, 0), 999999);

	position x = position(1, 0); //Check x axis
	position z = position(0, 1); //Check y axis

	if (!testAxis(x, self.x - self.halfWidth, self.x + self.halfWidth, other.x - other.halfWidth, other.x + other.halfWidth, mtv))
	{
		return false;
	}

	if (!testAxis(z, self.y - self.halfHeight, self.y + self.halfHeight, other.y - other.halfHeight, other.y + other.halfHeight, mtv))
	{
		return false;
	}

	float dirMag = sqrtf(mtv.direction.x * mtv.direction.x + mtv.direction.z * mtv.direction.z);
	mtv.direction = position(mtv.direction.x / dirMag, mtv.direction.z / dirMag);
	mtv.magnitude *= 1.001f; //Small multiplier to make it not look weird.
	
	return true;
}

bool Collision::testAxis(position axis, float minA, float maxA, float minB, float maxB, MTV& mtv)
{
	float axisLSq = axis.x * axis.x + axis.z * axis.z;

	if (axisLSq < 1.0e-8f)
	{
		return true;
	}

	float d0 = maxB - minA; //Left side
	float d1 = maxA - minB; //Right side

	if (d0 <= 0.0f || d1 <= 0.0f)
	{
		return false;
	}

	float overlap = (d0 < d1) ? d0 : -d1; //Where is the overlap

	position separation = position(axis.x * (overlap / axisLSq), axis.z * (overlap / axisLSq));

	float sepLSq = separation.x * separation.x + separation.z * separation.z;

	if (sepLSq < mtv.magnitude)
	{
		mtv.magnitude = sepLSq;
		mtv.direction = separation;
	}

	return true;
}