#pragma once
#include "ObjectTypes.h"
#include <random>

float randomInRange(float min, float max);

position normalise(position p);

position truncate(position p, float maxMag);

bool lineIntersection(position a, position b, position c, position d, position& pointOfIntersection);

float distSquared(position a, position b);

float roundToNearestHalf(float number);