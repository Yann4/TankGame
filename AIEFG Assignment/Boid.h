#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include "Collision.h"
#include "MathHelper.h"
#include "Bullet.h"
#include "Obstacle.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Bullet;
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

	int lives;
	position spawnPoint;
	position velocity;
	float maxVelocity = 1.0f;

	float rotation;
	float turnSpeed = 3.0f;

	Colour colour;

	float width, height;
	float bulletIndex = 0;
public:
	Boid();
	Boid(int id, position pos);
	Boid& operator=(const Boid&) = default;

	void Update(float delta);
	void Render();

	void UpdateState(position p, float rotation);

	void resolveCollision(position moveBy);

	Collision::BoundingBox getBoundingBox() { return Collision::BoundingBox(pos.x, pos.z, width, height); }
	BoidInfo getInfo() { return BoidInfo(id, pos, velocity, isTarget); }
	
	//Format is: "P:[id],[xPos],[zPos],[rotation]"
	std::string getInfoString() {
		return "P:" + std::to_string(id) + "," + std::to_string(pos.x).substr(0, 6) + "," + std::to_string(pos.z).substr(0, 6)
			+ "," + std::to_string(rotation).substr(0, 6);
	}

	void giveUpdateString(std::string actions, std::vector<Bullet*>& bullets);
	
	void hitByBullet();
};