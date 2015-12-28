#include "MathHelper.h"

#pragma once
#include "ObjectTypes.h"
#include <random>

float randomInRange(float min, float max)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(eng);
}

position normalise(position p)
{
	if (p.x == 0 && p.z == 0)
	{
		return position(0, 0);
	}
	float mag = sqrtf(p.x * p.x + p.z * p.z);
	p.x /= mag;
	p.z /= mag;
	return p;
}

position truncate(position p, float maxMag)
{
	float mag = sqrtf(p.x * p.x + p.z * p.z);
	if (mag > maxMag)
	{
		p.x /= mag;
		p.z /= mag;

		p.x *= maxMag;
		p.z *= maxMag;
	}
	return p;
}

bool lineIntersection(position a, position b, position c, position d, position& pointOfIntersection)
{
	position s1 = position(b.x - a.x, b.z - a.z);
	position s2 = position(d.x - c.x, d.z - c.z);

	float denominator = -s2.x * s1.z + s1.x * s2.z;

	//Lines are parallell
	if (denominator == 0)
	{
		return false;
	}

	float s = (-s1.z * (a.x - c.x) + s1.x * (a.z - c.z)) / denominator;
	float r = (s2.x * (a.z - c.z) - s2.z * (a.x - c.x)) / denominator;

	if (r >= 0 && r <= 1 && s >= 0 && s <= 1)
	{
		pointOfIntersection = position(a.x + (r * s1.x), a.z + (r * s1.z));
		return true;
	}

	return false;
}

float distSquared(position a, position b)
{
	float distSq = ((a.x - b.x) * (a.x - b.x)) + ((a.z - b.z) * (a.z - b.z));
	return abs(distSq);
}

float roundToNearestHalf(float number)
{
	return round(number * 2) / 2;
}