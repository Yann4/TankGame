#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include "Collision.h"
#include "MathHelper.h"

#define _USE_MATH_DEFINES
#include <math.h>

struct BoidInfo
{
	int id;
	position pos;
	position velocity;
	bool target;

	BoidInfo() :pos(position()), velocity(position()), id(-1), target(false)
	{}

	BoidInfo(int id, position pos, position vel, bool target) : id(id), pos(pos), velocity(vel), target(target)
	{}

	~BoidInfo() {}
};

class Boid
{

private:
	//General surface information
	int id;
	bool isTarget;
	position pos;

	position velocity;
	float maxVelocity = 2.0f;

	//Display/collision information info
	float rotation;

	Colour colour;

	float width, height;
public:
	Boid();
	Boid(int id, position pos);
	Boid& operator=(const Boid&) = default;

	void Update(float delta, const std::vector<BoidInfo>& others);
	void Render();
	
	void resolveCollision(position moveBy);

	Collision::BoundingBox getBoundingBox() { return Collision::BoundingBox(pos.x, pos.z, width, height); }
	BoidInfo getInfo() { return BoidInfo(id, pos, velocity, isTarget); }

private:
	void UpdateLocation(position steeringForce, float delta);
};